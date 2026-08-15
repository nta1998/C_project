#!/bin/bash
# ============================================================================
# run_e2e.sh - end-to-end tests for the whole assembler.
#
# Runs the built 'assembler' executable on complete .as source files and
# checks the actual output files (.am / .ob / .ent / .ext) against the
# expected/ directory. This exercises the full chain:
#     pre_assembler -> first_pass -> second_pass -> output
#
# The script never modifies a source file and never writes into Tests/.
# All assembling happens inside a temporary directory.
#
# The script's own labels are English. Text quoted from the sources is
# printed in full, with right-to-left runs pre-reversed by strip_rtl so it
# stays readable on terminals without BiDi support.
#
# Usage:
#   bash Tests/e2e/run_e2e.sh              run all tests
#   bash Tests/e2e/run_e2e.sh valid_data   run one test by name
#   bash Tests/e2e/run_e2e.sh --show       print the contents of every output file
#   bash Tests/e2e/run_e2e.sh --bless      record current output as expected
#
# Flags combine, e.g.  run_e2e.sh valid_data --show
# A failing case always prints its output files, with or without --show.
#
# Exit status: 0 if every test passed, 1 otherwise.
# ============================================================================

cd "$(dirname "$0")/../.." || exit 1

ASSEMBLER="./assembler"
CASE_DIR="Tests/e2e"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

RED=$'\033[31m'; GRN=$'\033[32m'; YEL=$'\033[33m'
BLD=$'\033[1m'; DIM=$'\033[2m'; RST=$'\033[0m'

PASS=0
FAIL=0
NOT_VERIFIED=0
FAILED_NAMES=""

BLESS=0
SHOW=0
ONLY=""
for arg in "$@"; do
    case "$arg" in
        --bless) BLESS=1 ;;
        --show)  SHOW=1 ;;
        "")      ;;
        *)       ONLY="$arg" ;;
    esac
done

# Tests whose sources contain deliberate errors. For these the assembler must
# report errors and must NOT leave any .ob/.ent/.ext behind.
is_error_case() {
    case "$1" in
        error_*) return 0 ;;
        *)       return 1 ;;
    esac
}

# Cases live in Tests/e2e/valids/ or Tests/e2e/errors/, split by the same
# rule as is_error_case. Each subdirectory carries its own expected/.
case_subdir() {
    if is_error_case "$1"; then printf 'errors'; else printf 'valids'; fi
}

# ---------------------------------------------------------------------------
# Make sure the assembler exists before running anything.
# ---------------------------------------------------------------------------
if [ ! -x "$ASSEMBLER" ]; then
    printf '%s\n' "${BLD}Building the assembler...${RST}"
    if ! make >"$WORK/build.log" 2>&1; then
        printf '%s\n' "${RED}BUILD FAILED${RST} - cannot run end-to-end tests."
        printf '%s\n' "The assembler executable could not be produced. Reason:"
        printf '\n'
        grep -E 'error|Error|undefined|multiple definition' "$WORK/build.log" | head -15
        printf '\n'
        printf '%s\n' "Fix the build first, then re-run this script."
        exit 1
    fi
fi

# Resolved only after the build, since the binary may not have existed before.
ASSEMBLER_ABS="$(cd "$(dirname "$ASSEMBLER")" && pwd)/$(basename "$ASSEMBLER")"

# ---------------------------------------------------------------------------
# compare_file <label> <actual> <expected>
# Prints a unified diff when the two differ. Returns 1 on mismatch.
# ---------------------------------------------------------------------------
compare_file() {
    local label="$1" actual="$2" expected="$3"

    # No baseline recorded yet. This is NOT a pass - nothing was verified.
    # It is reported separately so a missing baseline can never look green.
    if [ ! -f "$expected" ]; then
        if [ -f "$actual" ]; then
            printf '  %s\n' "${YEL}not verified${RST} $label was created, but no baseline exists (run --bless)"
            NOT_VERIFIED=$((NOT_VERIFIED + 1))
        fi
        return 0
    fi

    if [ ! -f "$actual" ]; then
        printf '  %s\n' "${RED}missing${RST} $label was not created"
        return 1
    fi

    if ! diff -q "$expected" "$actual" >/dev/null 2>&1; then
        printf '  %s\n' "${RED}differs${RST} $label"
        side_by_side "$expected" "$actual"
        return 1
    fi
    return 0
}

# ---------------------------------------------------------------------------
# side_by_side <expected> <actual>
# Shows the differing rows stacked - expected above actual - rather than in
# two columns. Two columns forced every message to be cut to ~32 characters,
# which hid the part that actually differed; stacking gives each line the
# full terminal width, so nothing is truncated.
#
# Only differing rows are printed, each with a caret marking the first column
# where the two strings part company. That column is usually the whole story
# (a wrong line number, a wrong opcode byte).
# ---------------------------------------------------------------------------
side_by_side() {
    local expected="$1" actual="$2"

    awk -v red="$RED" -v grn="$GRN" -v dim="$DIM" -v rst="$RST" \
        -v expfile="$expected" -v actfile="$actual" '
    BEGIN {
        ne = 0; na = 0
        while ((getline line < expfile) > 0) { e[++ne] = line }
        while ((getline line < actfile) > 0) { a[++na] = line }
        max = (ne > na) ? ne : na

        ndiff = 0
        for (i = 1; i <= max; i++) {
            le = (i <= ne) ? e[i] : "(missing - expected output has no such line)"
            la = (i <= na) ? a[i] : "(missing - the assembler printed nothing here)"
            if (le == la) { nsame++; continue }

            ndiff++
            # Cap the report. A run that differs everywhere would otherwise
            # scroll the useful first difference off the screen.
            if (ndiff > 15) continue

            printf "      %sline %d%s\n", red, i, rst
            printf "        expected: %s\n", le
            printf "        actual:   %s%s%s\n", red, la, rst

            # Point at the first differing character, when both sides exist.
            # The caret sits under the text itself, so the padding must match
            # the "        actual:   " prefix exactly - 8 spaces of indent
            # plus the 10-character label - or it points at the wrong byte.
            if (i <= ne && i <= na) {
                n = (length(le) < length(la)) ? length(le) : length(la)
                for (c = 1; c <= n; c++)
                    if (substr(le, c, 1) != substr(la, c, 1)) break
                printf "%*s%s^ first difference at column %d%s\n", \
                    18 + c - 1, "", dim, c, rst
            }
        }

        if (ndiff > 15)
            printf "      %s... and %d more differing line(s)%s\n", dim, ndiff - 15, rst
        if (nsame > 0)
            printf "      %s(%d line(s) matched)%s\n", dim, nsame, rst
    }
    '
}

# ---------------------------------------------------------------------------
# strip_rtl
# Reverses right-to-left runs so they survive a terminal without BiDi support.
#
# The test sources carry Hebrew comments. A terminal that does not implement
# the BiDi algorithm - the VSCode integrated terminal among them - prints a
# Hebrew run in byte order, which reads backwards. Pre-reversing each run
# cancels that out, so the text arrives readable.
#
# Nothing is hidden: every character of the line is printed. Only the order
# within an RTL run changes, and only to undo the terminal's own reordering.
# ---------------------------------------------------------------------------
strip_rtl() {
    perl -CSD -pe '
        # Hebrew block U+0590-U+05FF, plus marks and punctuation that belong
        # to the run. Adjacent spaces are included so words stay together;
        # trailing space is put back after the reversal.
        s/([\x{0590}-\x{05FF}][\x{0590}-\x{05FF}\s\x{200f}\x{200e}]*)/
            my $r = $1;
            my ($tail) = $r =~ \/(\s*)$\/;
            $r =~ s|\s*$||;
            scalar(reverse($r)) . $tail
        /ge;
    ' 2>/dev/null || cat
}

# ---------------------------------------------------------------------------
# show_outputs <sandbox> <name>
# Prints the contents of every output file the assembler produced.
# Called on --show, and automatically whenever a case fails, so a failure
# always comes with the evidence next to it.
# ---------------------------------------------------------------------------
show_outputs() {
    local sandbox="$1" name="$2"
    local found=0 ext f first

    for ext in am ob ent ext; do
        f="$sandbox/$name.$ext"
        [ -f "$f" ] || continue
        found=1

        printf '  %s\n' "${BLD}.$ext${RST}"
        if [ ! -s "$f" ]; then
            printf '      %s\n' "${YEL}(empty)${RST}"
            continue
        fi

        # The .ob header line is "<code bytes> <data bytes>", the rest are
        # "<address> <byte> <byte>..." rows. Label the header so it is not
        # mistaken for an address row.
        if [ "$ext" = "ob" ]; then
            first=1
            while IFS= read -r linetext; do
                if [ "$first" = "1" ]; then
                    printf '      %-28s %s\n' "$linetext" "${YEL}<- code bytes, data bytes${RST}"
                    first=0
                else
                    printf '      %s\n' "$linetext"
                fi
            done < "$f"
        else
            strip_rtl < "$f" | sed 's/^/      /'
        fi
    done

    if [ "$found" = "0" ]; then
        printf '  %s\n' "${YEL}no output files were produced${RST}"
    fi

    # stdout matters too: it carries the error report when one was printed.
    if [ -s "$sandbox/stdout.txt" ]; then
        printf '  %s\n' "${BLD}stdout${RST}"
        strip_rtl < "$sandbox/stdout.txt" | sed 's/^/      /'
    fi
}

# ---------------------------------------------------------------------------
# run_case <name>
# ---------------------------------------------------------------------------
run_case() {
    local name="$1"
    local subdir="$(case_subdir "$name")"
    local src="$CASE_DIR/$subdir/$name.as"
    local expected_dir="$CASE_DIR/$subdir/expected"
    local sandbox="$WORK/$name"
    local ok=1

    mkdir -p "$sandbox"
    cp "$src" "$sandbox/$name.as"

    # The assembler resolves output paths next to the input file, so run it
    # from inside the sandbox to keep Tests/ clean. The path must be absolute:
    # the sandbox is a temp directory, not a fixed depth below the project.
    ( cd "$sandbox" && "$ASSEMBLER_ABS" "$name.as" ) >"$sandbox/stdout.txt" 2>&1
    local status=$?

    printf '%s\n' "${BLD}$name${RST}"

    # A crash invalidates everything else, so check it first and stop here.
    # Signal deaths report as 128+signum (139 = SIGSEGV, 134 = SIGABRT).
    if [ "$status" -ge 128 ]; then
        local signame="signal $((status - 128))"
        [ "$status" = "139" ] && signame="SIGSEGV (segmentation fault)"
        [ "$status" = "134" ] && signame="SIGABRT"
        printf '  %s\n' "${RED}CRASHED${RST} the assembler died with $signame"
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        # Whatever was written before the crash is the best clue available.
        show_outputs "$sandbox" "$name"
        return
    fi

    if is_error_case "$name"; then
        # An error case must report at least one error...
        if ! grep -qE 'Error \[|ERRORS WERE FOUND' "$sandbox/stdout.txt"; then
            printf '  %s\n' "${RED}no errors reported${RST} but this file contains deliberate errors"
            ok=0
        fi
        # ...and must not leave output files behind.
        local leaked=""
        for ext in ob ent ext; do
            [ -f "$sandbox/$name.$ext" ] && leaked="$leaked .$ext"
        done
        if [ -n "$leaked" ]; then
            printf '  %s\n' "${RED}output leaked${RST} files created despite errors:$leaked"
            ok=0
        fi
        # The reported error list is still compared, so regressions show up.
        if [ "$BLESS" = "1" ]; then
            mkdir -p "$expected_dir"
            grep -oE 'Error \[[^]]*\]: .*' "$sandbox/stdout.txt" >"$expected_dir/$name.errors.txt"
        else
            grep -oE 'Error \[[^]]*\]: .*' "$sandbox/stdout.txt" >"$sandbox/errors.txt"
            compare_file "error list" "$sandbox/errors.txt" "$expected_dir/$name.errors.txt" || ok=0
        fi
    else
        # A valid case must report no errors at all.
        if grep -qE 'Error \[' "$sandbox/stdout.txt"; then
            printf '  %s\n' "${RED}unexpected errors${RST}"
            grep -E 'Error \[' "$sandbox/stdout.txt" | head -8 | strip_rtl | sed 's/^/      /'
            ok=0
        fi

        # A valid source must always produce an object file. Checked directly
        # so this fails even when no baseline has been recorded yet.
        if [ ! -f "$sandbox/$name.ob" ]; then
            printf '  %s\n' "${RED}no .ob produced${RST} a valid source must assemble to an object file"
            ok=0
        fi

        if [ "$BLESS" = "1" ]; then
            mkdir -p "$expected_dir"
            for ext in am ob ent ext; do
                [ -f "$sandbox/$name.$ext" ] && cp "$sandbox/$name.$ext" "$expected_dir/$name.$ext"
            done
        else
            for ext in am ob ent ext; do
                compare_file ".$ext" "$sandbox/$name.$ext" "$expected_dir/$name.$ext" || ok=0
            done
        fi
    fi

    if [ "$BLESS" = "1" ]; then
        printf '  %s\n' "${YEL}recorded${RST}"
        [ "$SHOW" = "1" ] && show_outputs "$sandbox" "$name"
        return 0
    fi

    if [ "$ok" = "1" ]; then
        printf '  %s\n' "${GRN}pass${RST}"
        PASS=$((PASS + 1))
        [ "$SHOW" = "1" ] && show_outputs "$sandbox" "$name"
    else
        FAIL=$((FAIL + 1))
        FAILED_NAMES="$FAILED_NAMES $name"
        # On failure always show what was produced - that is the evidence.
        show_outputs "$sandbox" "$name"
    fi
}

# ---------------------------------------------------------------------------
# Main loop
# ---------------------------------------------------------------------------
printf '%s\n\n' "${BLD}End-to-end tests${RST}"

if [ "$BLESS" = "1" ]; then
    printf '%s\n\n' "${YEL}--bless mode: recording current output as the expected baseline.${RST}"
    printf '%s\n\n' "${YEL}Review the recorded files by hand before trusting them.${RST}"
fi

found=0
for src in "$CASE_DIR"/valids/*.as "$CASE_DIR"/errors/*.as; do
    [ -f "$src" ] || continue
    name="$(basename "$src" .as)"
    [ -n "$ONLY" ] && [ "$name" != "$ONLY" ] && continue
    found=1
    run_case "$name"
done

if [ "$found" = "0" ]; then
    printf '%s\n' "${RED}No test case matched.${RST}"
    exit 1
fi

printf '\n'
if [ "$BLESS" = "1" ]; then
    printf '%s\n' "Baseline written to $CASE_DIR/valids/expected/ and $CASE_DIR/errors/expected/"
    exit 0
fi

printf '%s\n' "${BLD}$PASS passed, $FAIL failed${RST}"
if [ "$NOT_VERIFIED" -gt 0 ]; then
    printf '%s\n' "${YEL}$NOT_VERIFIED output file(s) were not verified - no baseline recorded.${RST}"
    printf '%s\n' "${YEL}A 'pass' here only means the assembler did not crash or report errors.${RST}"
    printf '%s\n' "${YEL}Run 'make e2e-bless' once the output is known to be correct.${RST}"
fi
if [ "$FAIL" -gt 0 ]; then
    printf '%s\n' "Failed:$FAILED_NAMES"
    exit 1
fi
exit 0
