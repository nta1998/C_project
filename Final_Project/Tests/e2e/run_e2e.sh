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
# Output is English on purpose: terminals without BiDi support (the VSCode
# integrated terminal among them) render Hebrew reversed.
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
EXPECTED_DIR="Tests/e2e/expected"
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
# Prints the two files in aligned columns with differing rows marked, so the
# wrong byte can be spotted by eye without reading diff syntax. Lines are
# compared by position: these are fixed-format output files, so row N of the
# expected output should be row N of the actual output.
# ---------------------------------------------------------------------------
side_by_side() {
    local expected="$1" actual="$2"

    awk -v red="$RED" -v dim="$DIM" -v rst="$RST" \
        -v expfile="$expected" -v actfile="$actual" '
    BEGIN {
        ne = 0; na = 0
        while ((getline line < expfile) > 0) { e[++ne] = line }
        while ((getline line < actfile) > 0) { a[++na] = line }
        max = (ne > na) ? ne : na

        # Column width follows the widest real line, clamped so that two
        # columns plus the gutters still fit an 80-column terminal.
        w = 5
        for (i = 1; i <= max; i++) {
            if (length(e[i]) > w) w = length(e[i])
            if (length(a[i]) > w) w = length(a[i])
        }
        if (w > 32) w = 32

        printf "      %-4s %-*s   %-*s\n", "line", w, "expected", w, "actual"
        printf "      %-4s %-*s   %-*s\n", "----", w, substr(dashes(w), 1, w), w, substr(dashes(w), 1, w)

        shown = 0
        for (i = 1; i <= max; i++) {
            le = (i <= ne) ? e[i] : "(missing)"
            la = (i <= na) ? a[i] : "(missing)"
            same = (le == la)

            # Identical rows are context. Show a few, then only differences,
            # so a one-byte error in a long file stays visible.
            if (same && shown >= 12) continue
            shown++

            if (same) {
                printf "      %-4d %-*s   %-*s\n", i, w, trunc(le, w), w, trunc(la, w)
            } else {
                printf "      %s%-4d%s %-*s   %s%-*s%s  %s<-%s\n", \
                    red, i, rst, w, trunc(le, w), red, w, trunc(la, w), rst, red, rst
            }
        }
        if (max > shown) printf "      %s... %d more identical line(s)%s\n", dim, max - shown, rst
    }
    function trunc(s, n) { return (length(s) > n) ? substr(s, 1, n - 1) "~" : s }
    function dashes(n,  s, i) { s = ""; for (i = 0; i < n; i++) s = s "-"; return s }
    '
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
            sed 's/^/      /' "$f"
        fi
    done

    if [ "$found" = "0" ]; then
        printf '  %s\n' "${YEL}no output files were produced${RST}"
    fi

    # stdout matters too: it carries the error report when one was printed.
    if [ -s "$sandbox/stdout.txt" ]; then
        printf '  %s\n' "${BLD}stdout${RST}"
        sed 's/^/      /' "$sandbox/stdout.txt"
    fi
}

# ---------------------------------------------------------------------------
# run_case <name>
# ---------------------------------------------------------------------------
run_case() {
    local name="$1"
    local src="$CASE_DIR/$name.as"
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
            mkdir -p "$EXPECTED_DIR"
            grep -oE 'Error \[[^]]*\]: .*' "$sandbox/stdout.txt" >"$EXPECTED_DIR/$name.errors.txt"
        else
            grep -oE 'Error \[[^]]*\]: .*' "$sandbox/stdout.txt" >"$sandbox/errors.txt"
            compare_file "error list" "$sandbox/errors.txt" "$EXPECTED_DIR/$name.errors.txt" || ok=0
        fi
    else
        # A valid case must report no errors at all.
        if grep -qE 'Error \[' "$sandbox/stdout.txt"; then
            printf '  %s\n' "${RED}unexpected errors${RST}"
            grep -E 'Error \[' "$sandbox/stdout.txt" | head -8 | sed 's/^/      /'
            ok=0
        fi

        # A valid source must always produce an object file. Checked directly
        # so this fails even when no baseline has been recorded yet.
        if [ ! -f "$sandbox/$name.ob" ]; then
            printf '  %s\n' "${RED}no .ob produced${RST} a valid source must assemble to an object file"
            ok=0
        fi

        if [ "$BLESS" = "1" ]; then
            mkdir -p "$EXPECTED_DIR"
            for ext in am ob ent ext; do
                [ -f "$sandbox/$name.$ext" ] && cp "$sandbox/$name.$ext" "$EXPECTED_DIR/$name.$ext"
            done
        else
            for ext in am ob ent ext; do
                compare_file ".$ext" "$sandbox/$name.$ext" "$EXPECTED_DIR/$name.$ext" || ok=0
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
for src in "$CASE_DIR"/*.as; do
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
    printf '%s\n' "Baseline written to $EXPECTED_DIR/"
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
