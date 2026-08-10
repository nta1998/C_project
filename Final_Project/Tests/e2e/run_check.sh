#!/bin/bash
# ============================================================================
# run_check.sh - diagnostic run for a single source file.
#
# Runs the assembler twice on the same input and reports both kinds of
# problem, because they are different failures with different causes:
#
#   1. Assembly errors  - problems in the .as source, reported by the
#                         assembler itself, located by line in the .as file.
#   2. Memory errors    - problems in the assembler's own C code, found by
#                         valgrind, located by line in the .c files.
#
# A file can assemble perfectly and still corrupt memory, so a clean run of
# part 1 says nothing about part 2. Both are always reported.
#
# The script never modifies a source file and never writes into Tests/.
# All assembling happens inside a temporary directory.
#
# Output is English on purpose: terminals without BiDi support (the VSCode
# integrated terminal among them) render Hebrew reversed.
#
# Usage:
#   bash Tests/e2e/run_check.sh valid_macro       a case name from Tests/e2e
#   bash Tests/e2e/run_check.sh path/to/file.as   any .as file
#   bash Tests/e2e/run_check.sh                   every case in Tests/e2e
#   bash Tests/e2e/run_check.sh valid_macro --no-mem    skip the valgrind run
#
# Exit status: 0 if no problem of either kind was found, 1 otherwise.
# ============================================================================

cd "$(dirname "$0")/../.." || exit 1

ASSEMBLER="./assembler"
CASE_DIR="Tests/e2e"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

RED=$'\033[31m'; GRN=$'\033[32m'; YEL=$'\033[33m'
BLD=$'\033[1m'; DIM=$'\033[2m'; RST=$'\033[0m'

RUN_MEM=1
TARGETS=""
for arg in "$@"; do
    case "$arg" in
        --no-mem) RUN_MEM=0 ;;
        "")       ;;
        *)        TARGETS="$TARGETS $arg" ;;
    esac
done

# valgrind is optional - the source-level check is still worth running
# without it, so a missing valgrind downgrades the run instead of failing it.
HAVE_VALGRIND=1
command -v valgrind >/dev/null 2>&1 || HAVE_VALGRIND=0

TOTAL_SRC_ERRORS=0
TOTAL_MEM_ERRORS=0

# ---------------------------------------------------------------------------
# Make sure the assembler exists before running anything.
# ---------------------------------------------------------------------------
if [ ! -x "$ASSEMBLER" ]; then
    printf '%s\n' "${BLD}Building the assembler...${RST}"
    if ! make >"$WORK/build.log" 2>&1; then
        printf '%s\n' "${RED}BUILD FAILED${RST} - cannot run the check."
        grep -E 'error|Error|undefined|multiple definition' "$WORK/build.log" | head -15
        exit 1
    fi
fi
ASSEMBLER_ABS="$(cd "$(dirname "$ASSEMBLER")" && pwd)/$(basename "$ASSEMBLER")"

# ---------------------------------------------------------------------------
# print_source_errors <sandbox> <name> <src>
# Shows every "Error [file:line]: message" the assembler printed, with the
# offending source line quoted underneath it. Seeing the actual text next to
# the message is what makes the error understandable without opening the file.
# ---------------------------------------------------------------------------
print_source_errors() {
    local sandbox="$1" name="$2" src="$3"
    local count line_no msg text

    grep -oE 'Error \[[^]]*\]: .*' "$sandbox/stdout.txt" >"$sandbox/errors.txt" 2>/dev/null
    count=$(grep -c . "$sandbox/errors.txt" 2>/dev/null)
    [ -z "$count" ] && count=0

    printf '  %s\n' "${BLD}Source errors${RST} ${DIM}(problems in the .as file)${RST}"

    if [ "$count" = "0" ]; then
        printf '    %s\n' "${GRN}none${RST}"
        return 0
    fi

    while IFS= read -r entry; do
        [ -n "$entry" ] || continue
        # "Error [name.as:8]: text"  ->  line number and message
        line_no=$(printf '%s' "$entry" | sed -n 's/^Error \[[^:]*:\([0-9]*\)\].*/\1/p')
        msg=$(printf '%s' "$entry" | sed 's/^Error \[[^]]*\]: //')

        if [ -n "$line_no" ]; then
            printf '    %s\n' "${RED}line ${line_no}${RST}  $msg"
            # Quote the offending line. An empty or absent line is called out
            # explicitly - silently printing nothing looks like a display bug,
            # when in fact it means the reported line number is suspect.
            total=$(wc -l < "$src")
            if [ "$line_no" -gt "$total" ]; then
                printf '    %s\n' "${DIM}       | ${RST}${YEL}(line $line_no is past the end of the file - only $total lines)${RST}"
            else
                text=$(sed -n "${line_no}p" "$src")
                if [ -z "$(printf '%s' "$text" | tr -d '[:space:]')" ]; then
                    printf '    %s\n' "${DIM}       | ${RST}${YEL}(this line is blank - the reported line number looks wrong)${RST}"
                else
                    printf '    %s\n' "${DIM}       | ${RST}$text"
                fi
            fi
        else
            printf '    %s\n' "${RED}error${RST}  $msg"
        fi
    done < "$sandbox/errors.txt"

    TOTAL_SRC_ERRORS=$((TOTAL_SRC_ERRORS + count))
    return 1
}

# ---------------------------------------------------------------------------
# print_memory_errors <sandbox> <name>
# Runs the assembler under valgrind and reports memory faults located in the
# project's own .c files. Frames from libc are dropped: they are almost always
# called from project code, and that calling line is the actionable one.
# ---------------------------------------------------------------------------
print_memory_errors() {
    local sandbox="$1" name="$2"
    local vg="$sandbox/valgrind.txt" nerr

    printf '  %s\n' "${BLD}Memory errors${RST} ${DIM}(problems in the assembler's C code)${RST}"

    if [ "$HAVE_VALGRIND" = "0" ]; then
        printf '    %s\n' "${YEL}skipped${RST} valgrind is not installed"
        return 0
    fi

    ( cd "$sandbox" && valgrind --error-exitcode=99 --track-origins=yes \
        --log-file=valgrind.txt "$ASSEMBLER_ABS" "$name.as" ) >/dev/null 2>&1

    # valgrind's own summary count is more reliable than counting stanzas.
    nerr=$(sed -n 's/.*ERROR SUMMARY: \([0-9]*\) errors.*/\1/p' "$vg" | tail -1)
    [ -z "$nerr" ] && nerr=0

    if [ "$nerr" = "0" ]; then
        printf '    %s\n' "${GRN}none${RST}"
        return 0
    fi

    # Each error stanza starts with a line that has no leading whitespace
    # after the pid prefix. Print the message and the project frames under it.
    # valgrind names frames as "func (file.c:37)" - no directory prefix - so
    # any frame carrying a .c line number is project code. Frames from libc
    # and the dynamic loader have no source location and drop out on their own.
    sed 's/^==[0-9]*== //' "$vg" | awk -v red="$RED" -v dim="$DIM" -v rst="$RST" '
        /^(Invalid|Conditional|Use of|Syscall|Mismatched|Source and|Uninitialised|Argument)/ {
            printf "    %s%s%s\n", red, $0, rst
            inblock = 1; next
        }
        /^ *(at|by) / && inblock {
            if ($0 ~ /\.c:[0-9]+\)/) {
                loc = $0
                sub(/^ *(at|by) 0x[0-9A-Fa-f]*: /, "", loc)
                printf "      %s\n", loc
            }
            next
        }
        # "Uninitialised value was created by ..." opens a second frame list
        # that says where the bad value came from - usually the real culprit.
        /^ *(Uninitialised value|Block was alloc|Address 0x)/ {
            printf "      %s%s%s\n", dim, $0, rst
            inblock = 1; next
        }
        /^ *$/ { inblock = 0 }
    '

    printf '    %s\n' "${DIM}full log: valgrind reported $nerr error(s)${RST}"
    TOTAL_MEM_ERRORS=$((TOTAL_MEM_ERRORS + nerr))
    return 1
}

# ---------------------------------------------------------------------------
# check_one <src-path>
# ---------------------------------------------------------------------------
check_one() {
    local src="$1"
    local name sandbox status

    name="$(basename "$src" .as)"
    sandbox="$WORK/$name"
    mkdir -p "$sandbox"
    cp "$src" "$sandbox/$name.as"

    ( cd "$sandbox" && "$ASSEMBLER_ABS" "$name.as" ) >"$sandbox/stdout.txt" 2>&1
    status=$?

    printf '%s\n' "${BLD}$name${RST}"

    # A crash is reported up front: it changes how everything below reads.
    if [ "$status" -ge 128 ]; then
        local signame="signal $((status - 128))"
        [ "$status" = "139" ] && signame="SIGSEGV (segmentation fault)"
        [ "$status" = "134" ] && signame="SIGABRT"
        printf '  %s\n' "${RED}CRASHED${RST} the assembler died with $signame"
        printf '  %s\n' "${DIM}the memory check below should point at the cause${RST}"
    fi

    print_source_errors "$sandbox" "$name" "$src"
    [ "$RUN_MEM" = "1" ] && print_memory_errors "$sandbox" "$name"
    printf '\n'
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
printf '%s\n\n' "${BLD}Diagnostic check${RST}"

found=0
if [ -z "$TARGETS" ]; then
    for src in "$CASE_DIR"/*.as; do
        [ -f "$src" ] || continue
        found=1
        check_one "$src"
    done
else
    for t in $TARGETS; do
        # Accept either a bare case name or a path to any .as file.
        if [ -f "$t" ]; then
            found=1; check_one "$t"
        elif [ -f "$CASE_DIR/$t.as" ]; then
            found=1; check_one "$CASE_DIR/$t.as"
        else
            printf '%s\n' "${RED}not found:${RST} $t"
        fi
    done
fi

if [ "$found" = "0" ]; then
    printf '%s\n' "${RED}No source file matched.${RST}"
    exit 1
fi

printf '%s\n' "${BLD}Summary${RST}"
printf '  source errors: %s\n' "$TOTAL_SRC_ERRORS"
if [ "$HAVE_VALGRIND" = "0" ]; then
    printf '  memory errors: %s\n' "${YEL}not checked (valgrind missing)${RST}"
elif [ "$RUN_MEM" = "0" ]; then
    printf '  memory errors: %s\n' "${YEL}not checked (--no-mem)${RST}"
else
    printf '  memory errors: %s\n' "$TOTAL_MEM_ERRORS"
fi

[ "$TOTAL_SRC_ERRORS" -gt 0 ] || [ "$TOTAL_MEM_ERRORS" -gt 0 ] && exit 1
exit 0
