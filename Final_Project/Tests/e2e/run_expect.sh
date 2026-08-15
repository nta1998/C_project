#!/bin/bash
# ============================================================================
# run_expect.sh - line-by-line comparison against a documented baseline.
#
# run_check.sh answers "how many errors". This answers "which line is wrong,
# and what should it have said" - so a failure points at the code to fix
# instead of only reporting a count.
#
# It reads the expectations straight out of the .expected.txt baseline, which
# already lists them as:
#
#     line  expected code   message
#     ----  -------------   ----------------------------------------
#     10    ERR_CODE_22     Invalid label name, does not start ...
#     16    (no error)      first definition of DUP -> symbol DUP = 100
#
# Parsing that file instead of keeping a second copy means the expectations
# live in exactly one place. Editing the baseline changes the test.
#
# Each source line is classified as:
#     ok       expected an error, got one           (code matched, if known)
#     WRONG    expected an error, got a different code
#     MISSED   expected an error, none was reported   <- a check is missing
#     EXTRA    expected no error, but one was reported <- a false positive
#
# The assembler prints messages, not ERR_CODE_n names, so codes are matched
# through the message text in Headers/errors.h. When a message cannot be
# resolved to a code the line is still checked for "was an error reported at
# all", and marked as unverified rather than silently passing.
#
# The script's own labels are English. Text quoted from the sources is
# printed in full, with right-to-left runs pre-reversed by strip_rtl so it
# stays readable on terminals without BiDi support.
#
# Usage:
#   bash Tests/e2e/run_expect.sh Tests/first_pass_errors.as
#   bash Tests/e2e/run_expect.sh Tests/first_pass_errors.as --baseline <file>
#   bash Tests/e2e/run_expect.sh          all files that have a baseline
#
# Exit status: 0 if every line matched, 1 otherwise.
# ============================================================================

cd "$(dirname "$0")/../.." || exit 1

ASSEMBLER="./assembler"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

RED=$'\033[31m'; GRN=$'\033[32m'; YEL=$'\033[33m'
BLD=$'\033[1m'; DIM=$'\033[2m'; RST=$'\033[0m'

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
# The filter lives in a real script file rather than a shell string: awk
# rewrites backslash escapes inside -v values, which corrupts the pattern.
# A file path passes through awk untouched.
RTL_FILTER="$WORK/rtl.pl"
cat >"$RTL_FILTER" <<'PERL'
#!/usr/bin/perl -CSD -p
s/([\x{0590}-\x{05FF}][\x{0590}-\x{05FF}\s\x{200f}\x{200e}]*)/
    my $r = $1;
    my ($tail) = $r =~ \/(\s*)$\/;
    $r =~ s|\s*$||;
    scalar(reverse($r)) . $tail
/ge;
PERL
chmod +x "$RTL_FILTER"

strip_rtl() {
    perl -CSD -p "$RTL_FILTER" 2>/dev/null || cat
}

BASELINE=""
TARGETS=""
prev=""
for arg in "$@"; do
    if [ "$prev" = "--baseline" ]; then BASELINE="$arg"; prev=""; continue; fi
    case "$arg" in
        --baseline) prev="--baseline" ;;
        "")         ;;
        *)          TARGETS="$TARGETS $arg" ;;
    esac
done

# ---------------------------------------------------------------------------
# Build if needed.
# ---------------------------------------------------------------------------
if [ ! -x "$ASSEMBLER" ]; then
    printf '%s\n' "${BLD}Building the assembler...${RST}"
    if ! make >"$WORK/build.log" 2>&1; then
        printf '%s\n' "${RED}BUILD FAILED${RST} - cannot compare against the baseline."
        grep -E 'error|undefined|multiple definition' "$WORK/build.log" | head -15
        exit 1
    fi
fi
ASSEMBLER_ABS="$(cd "$(dirname "$ASSEMBLER")" && pwd)/$(basename "$ASSEMBLER")"

# ---------------------------------------------------------------------------
# find_baseline <src>
# A baseline sits under Tests/expected, named after the case.
# ---------------------------------------------------------------------------
find_baseline() {
    local name base
    name="$(basename "$1" .as)"
    base="Tests/expected/$name.expected.txt"
    [ -f "$base" ] && { printf '%s' "$base"; return 0; }
    return 1
}

# ---------------------------------------------------------------------------
# build_code_map
# Maps each ERR_CODE_n to its message text, read from Headers/errors.h, so a
# reported message can be turned back into the code the baseline names.
# The table is written as: ERR_CODE_n<TAB>lowercased message
# ---------------------------------------------------------------------------
build_code_map() {
    awk '
        # Matches lines that pair a code with its string, in either order:
        #   {ERR_CODE_22, "Invalid label name..."}
        #   case ERR_CODE_22: ... "Invalid label name..."
        /ERR_CODE_[0-9]+/ {
            code = ""; msg = ""
            if (match($0, /ERR_CODE_[0-9]+/))
                code = substr($0, RSTART, RLENGTH)
            if (match($0, /"[^"]*"/))
                msg = substr($0, RSTART + 1, RLENGTH - 2)
            if (code != "" && msg != "") {
                print code "\t" tolower(msg)
            }
        }
    ' Sources/errors.c Headers/errors.h 2>/dev/null
}

# ---------------------------------------------------------------------------
# parse_baseline <baseline>
# Emits one record per documented line:  <line><TAB><code>
# "(no error)" becomes the literal NONE, so lines that must stay clean are
# checked just as strictly as lines that must fail.
# ---------------------------------------------------------------------------
parse_baseline() {
    awk '
        # "10    ERR_CODE_22     message..."
        /^[0-9]+[ \t]+ERR_CODE_[0-9]+/ {
            line = $1
            if (match($0, /ERR_CODE_[0-9]+/))
                print line "\t" substr($0, RSTART, RLENGTH)
            next
        }
        # "16    (no error)      ..."
        /^[0-9]+[ \t]+\(no error\)/ { print $1 "\tNONE" }
    ' "$1"
}

# ---------------------------------------------------------------------------
# check_one <src>
# ---------------------------------------------------------------------------
check_one() {
    local src="$1" baseline="$2"
    local name sandbox status
    local ok=0 wrong=0 missed=0 extra=0 unverified=0

    name="$(basename "$src" .as)"
    sandbox="$WORK/$name"
    mkdir -p "$sandbox"
    cp "$src" "$sandbox/$name.as"

    ( cd "$sandbox" && "$ASSEMBLER_ABS" "$name.as" ) >"$sandbox/stdout.txt" 2>&1
    status=$?

    printf '%s\n' "${BLD}$name${RST}  ${DIM}vs $baseline${RST}"

    if [ "$status" -ge 128 ]; then
        local signame="signal $((status - 128))"
        [ "$status" = "139" ] && signame="SIGSEGV (segmentation fault)"
        [ "$status" = "134" ] && signame="SIGABRT (stack smashing / abort)"
        printf '  %s\n' "${RED}CRASHED${RST} the assembler died with $signame"
        printf '  %s\n' "${DIM}nothing can be compared - fix the crash first${RST}"
        printf '  %s\n' "${DIM}run: make check FILE=$src   to locate it${RST}"
        printf '\n'
        return 1
    fi

    build_code_map >"$sandbox/codes.tsv"
    parse_baseline "$baseline" >"$sandbox/expect.tsv"

    if [ ! -s "$sandbox/expect.tsv" ]; then
        printf '  %s\n' "${YEL}no per-line expectations found in the baseline${RST}"
        printf '  %s\n' "${DIM}expected a table of '<line>  ERR_CODE_n  message' rows${RST}"
        printf '\n'
        return 1
    fi

    # Reported errors, as  <line><TAB><lowercased message>
    grep -oE 'Error \[[^]]*\]: .*' "$sandbox/stdout.txt" 2>/dev/null \
        | sed -E 's/^Error \[[^:]*:([0-9]+)\]: (.*)$/\1\t\2/' \
        | tr 'A-Z' 'a-z' >"$sandbox/actual.tsv"

    printf '  %-5s %-14s %-14s %s\n' "line" "expected" "actual" "source"
    printf '  %-5s %-14s %-14s %s\n' "-----" "--------------" "--------------" "------"

    # Compare line by line. awk holds the whole picture, which keeps the
    # verdicts consistent - each source line is judged exactly once.
    awk -F'\t' -v red="$RED" -v grn="$GRN" -v yel="$YEL" -v dim="$DIM" -v rst="$RST" \
        -v srcfile="$src" -v rtlfilter="$RTL_FILTER" '
    FILENAME == ARGV[1] { code2msg[$1] = $2; next }                 # codes.tsv
    FILENAME == ARGV[2] { expect[$1] = $2; order[++n] = $1; next }  # expect.tsv
    FILENAME == ARGV[3] {                                           # actual.tsv
        if ($1 in actual) actual[$1] = actual[$1] "; " $2
        else actual[$1] = $2
        next
    }
    END {
        for (i = 1; i <= n; i++) {
            ln = order[i]
            exp_code = expect[ln]
            act_msg  = (ln in actual) ? actual[ln] : ""

            # Quote the source line so the verdict is readable on its own.
            # Guarded: sed rejects address 0, and a baseline typo could
            # produce one - the error message would then drown the report.
            # Right-to-left runs are pre-reversed by the same perl filter the
            # shell side uses, so Hebrew comments read correctly here too.
            src_txt = ""
            if (ln + 0 >= 1) {
                cmd = "sed -n \"" ln "p\" " srcfile " | perl -CSD -p " rtlfilter
                cmd | getline src_txt
                close(cmd)
            }
            # Printed whole, comment included: the comment usually states
            # which error the line is meant to trigger, which is exactly what
            # you need when deciding whether the assembler or the test is wrong.
            sub(/^[ \t]+/, "", src_txt)

            if (exp_code == "NONE") {
                if (act_msg == "") {
                    printf "  %-5s %-14s %-14s %s\n", ln, "(none)", "(none)", src_txt
                    ok++
                } else {
                    printf "  %s%-5s%s %-14s %s%-14s%s %s\n", \
                        red, ln, rst, "(none)", red, "ERROR", rst, src_txt
                    printf "  %s      false positive: %s%s\n", dim, act_msg, rst
                    extra++
                }
                continue
            }

            if (act_msg == "") {
                printf "  %s%-5s%s %-14s %s%-14s%s %s\n", \
                    red, ln, rst, exp_code, red, "MISSED", rst, src_txt
                missed++
                continue
            }

            # Resolve the reported message back to a code, when possible.
            got = ""
            for (c in code2msg) {
                m = code2msg[c]
                if (m != "" && index(act_msg, m) > 0) { got = c; break }
            }

            if (got == "") {
                printf "  %s%-5s%s %-14s %s%-14s%s %s\n", \
                    yel, ln, rst, exp_code, yel, "reported", rst, src_txt
                printf "  %s      message not matched to a code: %s%s\n", dim, act_msg, rst
                unverified++
            } else if (got == exp_code) {
                printf "  %s%-5s%s %-14s %s%-14s%s %s\n", \
                    grn, ln, rst, exp_code, grn, got, rst, src_txt
                ok++
            } else {
                printf "  %s%-5s%s %-14s %s%-14s%s %s\n", \
                    red, ln, rst, exp_code, red, got, rst, src_txt
                wrong++
            }
        }
        printf "\n"
        printf "  %d matched, %s%d missed%s, %s%d wrong code%s, %s%d false positive%s",
               ok, (missed ? red : dim), missed, rst,
               (wrong ? red : dim), wrong, rst,
               (extra ? red : dim), extra, rst
        if (unverified > 0) printf ", %s%d unverified%s", yel, unverified, rst
        printf "\n"
        exit (missed + wrong + extra > 0) ? 1 : 0
    }
    ' "$sandbox/codes.tsv" "$sandbox/expect.tsv" "$sandbox/actual.tsv"

    local rc=$?

    # Errors on lines the baseline says nothing about. These are not failures
    # on their own - the baseline may simply be silent about them - but a
    # wrong line number shows up here, so they are always listed.
    cut -f1 "$sandbox/expect.tsv" | sort -u >"$sandbox/documented.txt"
    cut -f1 "$sandbox/actual.tsv" | sort -u >"$sandbox/reported.txt"
    if [ -s "$sandbox/reported.txt" ]; then
        comm -13 "$sandbox/documented.txt" "$sandbox/reported.txt" >"$sandbox/undoc.txt"
        if [ -s "$sandbox/undoc.txt" ]; then
            printf '\n  %s\n' "${YEL}errors on lines the baseline does not mention:${RST}"
            total="$(wc -l < "$src")"
            while IFS= read -r ln; do
                [ -n "$ln" ] || continue
                # Line 0 and past-the-end are impossible positions: the number
                # itself is the bug, so say that instead of quoting nothing.
                if [ "$ln" -lt 1 ] 2>/dev/null; then
                    printf '    line %-4s %s\n' "$ln" "${RED}(line 0 does not exist - the line counter is wrong)${RST}"
                    continue
                fi
                if [ "$ln" -gt "$total" ] 2>/dev/null; then
                    printf '    line %-4s %s\n' "$ln" "${RED}(past the end - the file has only $total lines)${RST}"
                    continue
                fi
                txt="$(sed -n "${ln}p" "$src" | strip_rtl)"
                if [ -z "$(printf '%s' "$txt" | tr -d '[:space:]')" ]; then
                    printf '    line %-4s %s\n' "$ln" "${RED}(this line is blank - the line number is wrong)${RST}"
                else
                    printf '    line %-4s %s\n' "$ln" "${DIM}$txt${RST}"
                fi
            done < "$sandbox/undoc.txt"
        fi
    fi

    printf '\n'
    return $rc
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
printf '%s\n\n' "${BLD}Expectation check${RST}"

FAILED=0
found=0

if [ -z "$TARGETS" ]; then
    for src in Tests/*.as Tests/e2e/valids/*.as Tests/e2e/errors/*.as; do
        [ -f "$src" ] || continue
        b="$(find_baseline "$src")" || continue
        found=1
        check_one "$src" "$b" || FAILED=1
    done
else
    for t in $TARGETS; do
        [ -f "$t" ] || { printf '%s %s\n' "${RED}not found:${RST}" "$t"; FAILED=1; continue; }
        if [ -n "$BASELINE" ]; then
            b="$BASELINE"
            [ -f "$b" ] || { printf '%s %s\n' "${RED}baseline not found:${RST}" "$b"; FAILED=1; continue; }
        else
            b="$(find_baseline "$t")" || {
                printf '%s\n' "${YEL}no baseline for $t${RST}"
                printf '%s\n' "${DIM}expected Tests/expected/$(basename "$t" .as).expected.txt${RST}"
                FAILED=1; continue
            }
        fi
        found=1
        check_one "$t" "$b" || FAILED=1
    done
fi

if [ "$found" = "0" ]; then
    printf '%s\n' "${RED}Nothing to check - no source had a baseline.${RST}"
    exit 1
fi

exit $FAILED
