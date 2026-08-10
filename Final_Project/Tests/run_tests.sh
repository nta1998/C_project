#!/bin/bash
# ============================================================================
# run_tests.sh - project-wide checks for the assembler.
#
# The script never modifies a source file. It compiles, links, runs functional
# tests against parser and first_pass, and prints a focused list of every
# problem it found.
#
# Output is English on purpose: terminals without BiDi support (the VSCode
# integrated terminal among them) render Hebrew reversed.
#
# Usage:
#   bash Tests/run_tests.sh            check the whole project
#   bash Tests/run_tests.sh --mine     check only the files listed in MY_FILES
# ============================================================================

cd "$(dirname "$0")/.." || exit 1

CC="gcc"
CFLAGS="-Wall -ansi -pedantic"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

RED=$'\033[31m'; GRN=$'\033[32m'; YEL=$'\033[33m'; BLD=$'\033[1m'; RST=$'\033[0m'

# The files you own. --mine limits the whole report to these.
MY_FILES="Sources/first_pass.c Sources/parser.c"
MY_HEADERS="Headers/first_pass.h Headers/parser.h"
MINE_ONLY=0
[ "$1" = "--mine" ] && MINE_ONLY=1

PROBLEMS=()
PASS=0
FAIL=0

problem() { PROBLEMS+=("$1|$2|$3"); }          # category | location | description
ok()      { PASS=$((PASS+1)); printf "  ${GRN}PASS${RST}  %s\n" "$1"; }
bad()     { FAIL=$((FAIL+1)); printf "  ${RED}FAIL${RST}  %s\n" "$1"; }
skip()    { printf "  ${YEL}SKIP${RST}  %s\n" "$1"; }

section() { printf "\n${BLD}%s${RST}\n" "$1"; }

# ============================================================================
if [ "$MINE_ONLY" -eq 1 ]; then
    section "0. Header sanity - $MY_HEADERS"

    for hdr in $MY_HEADERS; do
        base=$(basename "$hdr")

        # a. does the header compile on its own, without relying on someone
        #    else's #include coming first?
        echo "#include \"$base\"" > "$WORK/hchk.c"
        if $CC $CFLAGS -I Headers -c "$WORK/hchk.c" -o /dev/null 2>"$WORK/hchk.log"; then
            ok "$hdr compiles standalone"
        else
            bad "$hdr does not compile standalone"
            while IFS= read -r er; do
                loc=$(echo "$er" | sed 's/^In file included from.*//' | cut -d: -f1-2)
                msg=$(echo "$er" | sed 's/^[^:]*:[0-9]*:[0-9]*: error: //')
                [ -n "$msg" ] && problem "headers" "${loc:-$hdr}" "$msg"
            done < <(grep "error:" "$WORK/hchk.log")
        fi

        # b. include guard
        if grep -q '^#ifndef' "$hdr" && grep -q '^#define' "$hdr" && grep -q '^#endif' "$hdr"; then
            ok "$hdr has an include guard"
        else
            bad "$hdr is missing its include guard"
            problem "headers" "$hdr" "no include guard (#ifndef / #define / #endif)"
        fi
    done

    # c. functions defined in the .c with no prototype in the .h
    #    (those should normally be static)
    for src in $MY_FILES; do
        $CC $CFLAGS -Wmissing-prototypes -Wstrict-prototypes -I Headers \
            -c "$src" -o /dev/null 2>"$WORK/proto.log"
        nproto=$(grep -c "no previous prototype\|non-prototype" "$WORK/proto.log")
        if [ "$nproto" -eq 0 ]; then
            ok "$src - every function is declared"
        else
            bad "$src - $nproto function(s) with no prototype"
            while IFS= read -r w; do
                loc=$(echo "$w" | cut -d: -f1-2)
                msg=$(echo "$w" | sed 's/^[^:]*:[0-9]*:[0-9]*: warning: //')
                problem "headers" "$loc" "$msg"
            done < <(grep "no previous prototype\|non-prototype" "$WORK/proto.log" | sort -u)
        fi
    done
fi

# ============================================================================
if [ "$MINE_ONLY" -eq 1 ]; then
    section "1. Compile ($CFLAGS) - $MY_FILES only"
    SRC_LIST="$MY_FILES"
else
    section "1. Compile each source file ($CFLAGS)"
    SRC_LIST="$(ls Sources/*.c)"
fi
# ============================================================================

COMPILE_OK=1
for src in $SRC_LIST; do
    log="$WORK/$(basename "$src").log"
    if $CC $CFLAGS -c "$src" -o "$WORK/$(basename "$src").o" 2>"$log"; then
        nwarn=$(grep -c "warning:" "$log")
        if [ "$nwarn" -eq 0 ]; then
            ok "$src"
        else
            bad "$src - $nwarn warning(s)"
            while IFS= read -r w; do
                loc=$(echo "$w" | cut -d: -f1-2)
                msg=$(echo "$w" | sed 's/^[^:]*:[0-9]*:[0-9]*: warning: //')
                problem "compile warning" "$loc" "$msg"
            done < <(grep "warning:" "$log" | sort -u)
        fi
    else
        COMPILE_OK=0
        bad "$src - compile error"
        while IFS= read -r er; do
            loc=$(echo "$er" | cut -d: -f1-2)
            msg=$(echo "$er" | sed 's/^[^:]*:[0-9]*:[0-9]*: error: //')
            problem "compile error" "$loc" "$msg"
        done < <(grep "error:" "$log")
    fi
done

# ============================================================================
section "2. Link the assembler executable"
# ============================================================================

if [ "$MINE_ONLY" -eq 1 ]; then
    skip "depends on files you do not own"
elif [ "$COMPILE_OK" -eq 1 ]; then
    objs=$(ls "$WORK"/*.o 2>/dev/null | grep -v -e 'main\.c\.o' -e 'fp_main\.c\.o')
    if $CC $CFLAGS $objs -o "$WORK/assembler" 2>"$WORK/link.log"; then
        ok "link succeeded"
    else
        bad "link failed"
        while IFS= read -r sym; do
            problem "link" "assembler" "undefined reference to \`$sym'"
        done < <(grep -o "undefined reference to \`[^']*'" "$WORK/link.log" |
                 sed "s/undefined reference to \`//;s/'//" | sort -u)
    fi
else
    bad "skipped - compilation failed"
    problem "link" "assembler" "not checked, there are compile errors"
fi

# ============================================================================
section "3. parser unit tests"
# ============================================================================

# self-contained driver, written to a temp dir and not to the project
cat > "$WORK/pdrv.c" <<'EOF'
#include <stdio.h>
#include "parser.h"
int main(int argc, char *argv[])
{
    FILE *f; char buf[300]; int n = 0; Parsed_line pl;
    if (argc < 2) return 1;
    f = fopen(argv[1], "r"); if (!f) return 1;
    while (fgets(buf, sizeof(buf), f) != NULL) {
        n++;
        if (line_split(buf, &pl, argv[1], n))
            printf("%d|OK|%d|%s|%s|%s\n", n, (int)pl.kind, pl.label, pl.name, pl.rest);
        else
            printf("%d|REJECT|%d|%s|%s|%s\n", n, (int)pl.kind, pl.label, pl.name, pl.rest);
    }
    fclose(f); return 0;
}
EOF

PARSER_DRV=""
if $CC $CFLAGS -I Headers "$WORK/pdrv.c" Sources/parser.c Sources/instructions.c \
        Sources/errors.c -o "$WORK/pdrv" 2>"$WORK/pdrv.log"; then
    PARSER_DRV="$WORK/pdrv"
    ok "parser driver built"
else
    bad "parser driver did not build"
    problem "parser" "Sources/parser.c" "cannot build a test driver - see compile errors"
fi

# test cases:  input ~ expected     status|kind|label|name|rest
# kind:  0=EMPTY 1=COMMENT 2=DIRECTIVE 3=INSTRUCTION 4=INVALID
PCASES=(
  'hlt~OK|3||hlt|'
  'add $3,$19,$8~OK|3||add|$3,$19,$8'
  'LOOP: add $1,$2,$3~OK|3|LOOP|add|$1,$2,$3'
  '        sub  $1,$2,$3~OK|3||sub|$1,$2,$3'
  '.entry MAIN~OK|2||.entry|MAIN'
  '.extern val1~OK|2||.extern|val1'
  'STR: .asciz "abc"~OK|2|STR|.asciz|"abc"'
  'K: .dw 31,-12~OK|2|K|.dw|31,-12'
  '; a comment~OK|1|||'
  '   ~OK|0|||'
  'add $1,$2,$3 ; trailing comment~OK|3||add|$1,$2,$3'
  'S: .asciz "a;b"~OK|2|S|.asciz|"a;b"'
  '1abc: hlt~REJECT'
  'addd $1,$2,$3~REJECT'
  'LOOP add $1,$2,$3~REJECT'
  '.dx 5~REJECT'
)

if [ -n "$PARSER_DRV" ]; then
    : > "$WORK/pin.as"
    for c in "${PCASES[@]}"; do printf '%s\n' "${c%%~*}" >> "$WORK/pin.as"; done
    "$PARSER_DRV" "$WORK/pin.as" 2>/dev/null > "$WORK/pout.txt"

    idx=0
    for c in "${PCASES[@]}"; do
        idx=$((idx+1))
        input="${c%%~*}"; expect="${c#*~}"
        got=$(grep "^$idx|" "$WORK/pout.txt" | cut -d'|' -f2-)
        if [ "$expect" = "REJECT" ]; then
            status=$(echo "$got" | cut -d'|' -f1)
            kind=$(echo "$got" | cut -d'|' -f2)
            if [ "$status" = "REJECT" ] || [ "$kind" = "4" ]; then
                ok "rejected as expected: '$input'"
            else
                bad "'$input' - got $got, expected a rejection"
                problem "parser" "Sources/parser.c (line_split)" \
                        "illegal line '$input' was accepted as valid (kind=$kind)"
            fi
        else
            if [ "$got" = "$expect" ]; then
                ok "'$input'"
            else
                bad "'$input'"
                problem "parser" "Sources/parser.c (line_split)" \
                        "'$input' -> got [$got], expected [$expect]"
            fi
        fi
    done
fi

# ============================================================================
section "4. first_pass tests"
# ============================================================================

cat > "$WORK/fdrv.c" <<'EOF'
#include <stdio.h>
#include "globals.h"
#include "first_pass.h"
#include "errors.h"
#include "symbol_table.h"
#include "memory_image.h"
int main(int argc, char *argv[])
{
    FILE *am; long icf = 0, dcf = 0; Bool ok; Symbol *s; int i;
    const CODE_LINE *code;
    if (argc < 2) return 2;
    am = fopen(argv[1], "r"); if (!am) return 2;
    err_reset(); symbol_table_reset(); memory_image_reset();
    ok = first_pass(am, argv[1], NULL, NULL, NULL, &icf, &dcf);
    fclose(am);
    printf("RET=%s\n", ok ? "TRUE" : "FALSE");
    printf("ICF=%ld\nDCF=%ld\n", icf, dcf);
    for (s = symbol_table_get_head(); s != NULL; s = s->next)
        printf("SYM=%s,%ld,%d\n", s->symbol_name, s->value, (int)s->attribute);
    code = get_code_image();
    for (i = 0; i < get_IC_count(); i++)
        printf("CODE=%d\n", code[i].address);
    printf("NCODE=%d\nNDATA=%d\n", get_IC_count(), get_DC_count());
    return 0;
}
EOF

FP_DRV=""
if $CC $CFLAGS -I Headers "$WORK/fdrv.c" Sources/first_pass.c Sources/parser.c \
        Sources/instructions.c Sources/errors.c Sources/symbol_table.c \
        Sources/memory_image.c -o "$WORK/fdrv" 2>"$WORK/fdrv.log"; then
    FP_DRV="$WORK/fdrv"
    ok "first_pass driver built"
else
    bad "first_pass driver did not build"
    problem "first_pass" "Sources/first_pass.c" \
            "cannot build a test driver - see compile errors"
fi

# printf calls with no '\n' - their output runs into whatever prints next
for src in $SRC_LIST; do
    while IFS= read -r hit; do
        ln="${hit%%:*}"
        txt=$(echo "${hit#*:}" | sed 's/^[[:space:]]*//')
        problem "printf without \\n" "$src:$ln" "output runs into the next print: $txt"
    done < <(grep -n 'printf *( *"[^"]*"' "$src" | grep -v '\\n' | grep -v 'Error \[')
done

VALID="Tests/first_pass_valid.as"
ERRS="Tests/first_pass_errors.as"

# --- warn about lines longer than 80 BYTES (a Hebrew char is 2 bytes in UTF-8) ---
for f in "$VALID" "$ERRS"; do
    [ -f "$f" ] || continue
    long=$(awk 'length($0) > 80 {printf "%d ", NR}' "$f")
    if [ -n "$long" ]; then
        problem "test file" "$f" \
                "lines longer than 80 bytes: $long (a Hebrew char is 2 bytes in UTF-8)"
    fi
done

check_val() { # label | expected | actual | location
    if [ "$2" = "$3" ]; then
        ok "$1 = $3"
    else
        bad "$1: expected $2, got $3"
        problem "first_pass" "$4" "$1 came out $3 instead of $2"
    fi
}

if [ -n "$FP_DRV" ] && [ -f "$VALID" ]; then
    printf "\n  ${BLD}%s${RST}\n" "-- $VALID (must pass with no errors) --"
    # comment-stripped copy, so the UTF-8 line length issue does not mask
    # the checks that actually matter here
    sed 's/;.*$//' "$VALID" > "$WORK/valid_ascii.as"

    if timeout 10 "$FP_DRV" "$WORK/valid_ascii.as" > "$WORK/valid.out" 2>&1; then
        RET=$(grep -o 'RET=[A-Z]*'  "$WORK/valid.out" | head -1 | cut -d= -f2)
        ICF=$(grep -o 'ICF=-\?[0-9]*' "$WORK/valid.out" | head -1 | cut -d= -f2)
        DCF=$(grep -o 'DCF=-\?[0-9]*' "$WORK/valid.out" | head -1 | cut -d= -f2)
        check_val "return value" "TRUE" "$RET" "Sources/first_pass.c"
        check_val "ICF" "168" "$ICF" "Sources/first_pass.c"
        check_val "DCF" "19"  "$DCF" "Sources/first_pass.c"

        nerr=$(grep -c '^Error' "$WORK/valid.out")
        if [ "$nerr" -eq 0 ]; then
            ok "no errors reported"
        else
            bad "$nerr error(s) reported on a valid file"
            while IFS= read -r l; do
                problem "first_pass" "$VALID" "spurious error on a valid file: $l"
            done < <(grep '^Error' "$WORK/valid.out")
        fi

        # symbol table after the shift
        for exp in "MAIN,100,0" "LOOP,112,0" "END,164,0" "STR,168,1" \
                   "LIST,172,1" "HALF,175,1" "K,179,1" "vall,0,2"; do
            name="${exp%%,*}"
            if grep -q "^SYM=$exp$" "$WORK/valid.out"; then
                ok "symbol $exp"
            else
                actual=$(grep -o "SYM=$name:\?,[^ ]*" "$WORK/valid.out" | head -1 | cut -d= -f2)
                bad "symbol $name: expected [$exp], got [${actual:-missing}]"
                problem "first_pass" "Sources/first_pass.c (symbol table)" \
                        "symbol $name should be [$exp], got [${actual:-not in the table}]"
            fi
        done
    else
        bad "run failed (crash or timeout)"
        problem "first_pass" "Sources/first_pass.c" \
                "crash while processing $VALID - run it under gdb for details"
    fi
fi

if [ -n "$FP_DRV" ] && [ -f "$ERRS" ]; then
    printf "\n  ${BLD}%s${RST}\n" "-- $ERRS (every line holds one error) --"
    cp "$ERRS" "$WORK/errs_in.as"

    if timeout 10 "$FP_DRV" "$WORK/errs_in.as" > "$WORK/errs.out" 2>&1; then
        RET=$(grep -o 'RET=[A-Z]*' "$WORK/errs.out" | head -1 | cut -d= -f2)
        check_val "return value" "FALSE" "$RET" "Sources/first_pass.c"

        # the lines the test file itself declares as faulty
        EXPECTED_LINES=$(grep -n 'ERR_CODE_[0-9]' "$ERRS" | grep -v '^[0-9]*:;' |
                         cut -d: -f1 | sort -n | uniq)
        missed=""
        for ln in $EXPECTED_LINES; do
            grep -q "^Error \[[^]]*:$ln\]" "$WORK/errs.out" || missed="$missed $ln"
        done

        nexp=$(echo "$EXPECTED_LINES" | wc -w)
        ngot=$(grep -c '^Error' "$WORK/errs.out")
        if [ -z "$missed" ]; then
            ok "all $nexp faulty lines were caught"
        else
            bad "no error reported on lines:$missed"
            for ln in $missed; do
                txt=$(sed -n "${ln}p" "$ERRS" | sed 's/;.*$//' |
                      sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
                code=$(sed -n "${ln}p" "$ERRS" | grep -o 'ERR_CODE_[0-9]*')
                problem "first_pass" "$ERRS:$ln" \
                        "missed $code on: $txt"
            done
        fi
        printf "     total: %s expected, %s reported\n" "$nexp" "$ngot"
    else
        bad "run failed (crash or timeout)"
        problem "first_pass" "Sources/first_pass.c" \
                "crash while processing $ERRS - run it under gdb for details"
    fi
fi

# ============================================================================
section "5. Memory checks (AddressSanitizer + UBSan)"
# ============================================================================

if $CC -g -fsanitize=address,undefined -I Headers "$WORK/fdrv.c" Sources/first_pass.c \
        Sources/parser.c Sources/instructions.c Sources/errors.c \
        Sources/symbol_table.c Sources/memory_image.c -o "$WORK/asan" 2>/dev/null; then
    for f in "$VALID" "$ERRS" Tests/parser_test.as; do
        [ -f "$f" ] || continue
        out=$("$WORK/asan" "$f" 2>&1)
        if echo "$out" | grep -q "AddressSanitizer\|runtime error"; then
            bad "$f - memory violation"
            loc=$(echo "$out" | grep -o "Sources/[a-z_]*\.c:[0-9]*" | head -1)
            kind=$(echo "$out" | grep -o "ERROR: AddressSanitizer: [a-z-]*" | head -1)
            [ -z "$kind" ] && kind=$(echo "$out" | grep -o "runtime error: .*" | head -1)
            problem "memory" "${loc:-$f}" "${kind:-violation} while processing $f"
        else
            ok "$f clean"
        fi
    done
else
    bad "could not build with sanitizers"
fi

# ============================================================================
# Summary
# ============================================================================

printf "\n${BLD}================================================================${RST}\n"
printf "${BLD} Summary${RST}\n"
printf "${BLD}================================================================${RST}\n"
printf "  passed: ${GRN}%d${RST}    failed: ${RED}%d${RST}\n" "$PASS" "$FAIL"

if [ ${#PROBLEMS[@]} -eq 0 ]; then
    printf "\n${GRN}${BLD}No problems found.${RST}\n\n"
    exit 0
fi

printf "\n${BLD}%d problem(s) found:${RST}\n" "${#PROBLEMS[@]}"

last_cat=""
n=0
for cat in "headers" "compile error" "link" "parser" "first_pass" "memory" \
           "printf without \\n" "compile warning" "test file"; do
    for p in "${PROBLEMS[@]}"; do
        c="${p%%|*}"
        [ "$c" = "$cat" ] || continue
        rest="${p#*|}"
        loc="${rest%%|*}"
        desc="${rest#*|}"
        if [ "$cat" != "$last_cat" ]; then
            printf "\n  ${YEL}${BLD}%s${RST}\n" "$cat"
            last_cat="$cat"
        fi
        n=$((n+1))
        printf "   %2d. ${BLD}%s${RST}\n       %s\n" "$n" "$loc" "$desc"
    done
done

printf "\n"
exit 1
