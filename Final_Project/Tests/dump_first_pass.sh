#!/bin/bash
# ============================================================================
# dump_first_pass.sh - print what first_pass() actually produced, laid out the
# same way as Tests/expected/*.expected.txt so the two can be compared.
#
#   bash Tests/dump_first_pass.sh Tests/first_pass_valid.as
#       print the actual result
#
#   bash Tests/dump_first_pass.sh Tests/first_pass_valid.as --diff
#       normalise both sides and diff them field by field
#
#   bash Tests/dump_first_pass.sh Tests/first_pass_valid.as --raw
#       add the machine code words to the code image
#
# Comments in the .as file are stripped before the run, because a Hebrew
# comment pushes a line past 80 BYTES even when it is short in characters.
# Pass --keep-comments to feed the file through untouched.
# ============================================================================

cd "$(dirname "$0")/.." || exit 1

CC="gcc"
CFLAGS="-Wall -ansi -pedantic -g"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

AS_FILE=""
MODE="show"
KEEP_COMMENTS=0
RAW=0

for a in "$@"; do
    case "$a" in
        --diff)           MODE="diff" ;;
        --raw)            RAW=1 ;;
        --keep-comments)  KEEP_COMMENTS=1 ;;
        -*)               echo "unknown option: $a" >&2; exit 2 ;;
        *)                AS_FILE="$a" ;;
    esac
done

[ -z "$AS_FILE" ] && AS_FILE="Tests/first_pass_valid.as"

if [ ! -f "$AS_FILE" ]; then
    echo "no such file: $AS_FILE" >&2
    exit 2
fi

# ---------------------------------------------------------------- driver ----
cat > "$WORK/dump.c" <<'EOF'
#include <stdio.h>
#include "globals.h"
#include "first_pass.h"
#include "errors.h"
#include "symbol_table.h"
#include "memory_image.h"

int main(int argc, char *argv[])
{
    FILE *am;
    long icf = 0, dcf = 0;
    Bool ok;
    Symbol *s;
    int i;
    const CODE_LINE *code;
    const DATA_LINE *data;

    if (argc < 2) return 2;
    am = fopen(argv[1], "r");
    if (am == NULL) return 2;

    err_reset();
    symbol_table_reset();
    memory_image_reset();

    /* everything first_pass() prints itself lands here, between the markers */
    printf("<<<CONSOLE\n");
    ok = first_pass(am, argv[1], NULL, NULL, NULL, &icf, &dcf);
    printf("\nCONSOLE>>>\n");
    fclose(am);

    printf("RET %s\n", ok ? "TRUE" : "FALSE");
    printf("ICF %ld\n", icf);
    printf("DCF %ld\n", dcf);

    code = get_code_image();
    for (i = 0; i < get_IC_count(); i++)
        printf("CODE %d %08lX %s\n", code[i].address,
               code[i].machine_code, code[i].source_code);

    data = get_data_image();
    for (i = 0; i < get_DC_count(); i++)
        printf("DATA %d %lu %d\n", data[i].address,
               data[i].machine_code, data[i].byte_size);

    for (s = symbol_table_get_head(); s != NULL; s = s->next)
        printf("SYM %s %ld %d %s\n", s->symbol_name, s->value,
               (int)s->attribute, s->is_entry ? "TRUE" : "FALSE");

    return 0;
}
EOF

if ! $CC $CFLAGS -I Headers "$WORK/dump.c" Sources/first_pass.c Sources/parser.c \
        Sources/instructions.c Sources/errors.c Sources/symbol_table.c \
        Sources/memory_image.c -o "$WORK/dump" 2>"$WORK/build.log"; then
    echo "the driver did not build - first_pass.c or parser.c has a compile error:"
    echo
    grep -E "error:" "$WORK/build.log" | head -10
    exit 2
fi

# ------------------------------------------------------------- input prep ---
IN="$WORK/in.as"
if [ "$KEEP_COMMENTS" -eq 1 ]; then
    cp "$AS_FILE" "$IN"
else
    sed 's/;.*$//' "$AS_FILE" > "$IN"
fi

timeout 10 "$WORK/dump" "$IN" > "$WORK/out.txt" 2>&1
RC=$?
if [ $RC -ne 0 ]; then
    echo "the run failed (exit $RC$([ $RC -eq 124 ] && echo ', timeout'))."
    echo "last lines before it died:"
    tail -5 "$WORK/out.txt"
    exit 2
fi

# console output that first_pass() printed itself
sed -n '/^<<<CONSOLE$/,/^CONSOLE>>>$/p' "$WORK/out.txt" |
    sed '1d;$d' | sed '/^$/d' > "$WORK/console.txt"
grep -E '^(RET|ICF|DCF|CODE|DATA|SYM) ' "$WORK/out.txt" > "$WORK/fields.txt"

ATTR_NAME=(S_CODE S_DATA S_EXTERNAL)

# turn a stored unsigned value back into the signed number it represents
to_signed() { # value | byte_size
    python3 -c "
v=int('$1'); b=int('$2')*8
m=1<<b
v&=m-1
print(v-m if v>=(m>>1) else v)"
}

# ============================================================== show mode ===
# The table itself is rendered by Tests/render_first_pass.py, so that the
# formatting logic is not buried inside a here-document.
render() {
    python3 Tests/render_first_pass.py "$WORK/fields.txt" "$WORK/console.txt" "$AS_FILE"
}

# ============================================================== diff mode ===
# Both sides are reduced to the same canonical lines, so the comparison is on
# facts rather than on spacing or on the prose in the expected file.
canon_actual() {
    awk '$1=="RET"{print "RET " $2}
         $1=="ICF"{print "ICF " $2}
         $1=="DCF"{print "DCF " $2}' "$WORK/fields.txt"
    awk '$1=="CODE"{printf "CODE %s", $2; for(i=4;i<=NF;i++) printf " %s", $i; print ""}' \
        "$WORK/fields.txt" | sed 's/[[:space:]]\+/ /g;s/ $//'
    while read -r _ addr val size; do
        echo "DATA $addr $(to_signed "$val" "$size") $size"
    done < <(grep '^DATA ' "$WORK/fields.txt")
    while read -r _ name val attr entry; do
        echo "SYM $name $val ${ATTR_NAME[$attr]}"
    done < <(grep '^SYM ' "$WORK/fields.txt")
}

canon_expected() {
    python3 - "$1" <<'PYEOF'
import re, sys

text = open(sys.argv[1], encoding='utf-8', errors='replace').read().splitlines()
section = None
out = []

def char_to_int(tok):
    tok = tok.strip()
    if tok == r"'\0'":
        return 0
    m = re.fullmatch(r"'(.)'", tok)
    if m:
        return ord(m.group(1))
    return int(tok)

for line in text:
    s = line.strip()
    if s.startswith('Return value:'):
        out.append('RET ' + s.split(':', 1)[1].split()[0])
        continue
    m = re.match(r'ICF[^=]*=\s*(-?\d+)', s)
    if m:
        out.append('ICF ' + m.group(1)); continue
    m = re.match(r'DCF[^=]*=\s*(-?\d+)', s)
    if m:
        out.append('DCF ' + m.group(1)); continue

    if s.startswith('Code image'):    section = 'code';  continue
    if s.startswith('Data image'):    section = 'data';  continue
    if s.startswith('Symbol table'):  section = 'sym';   continue
    if s.startswith('Console output'): section = 'con';  continue
    if set(s) in ({'-'}, {'='}) or not s:
        continue

    if section == 'code':
        m = re.match(r'(\d+)\s+(.*)$', s)
        if m:
            body = re.sub(r'\s+', ' ', m.group(2).split(';')[0]).strip()
            out.append('CODE %s %s' % (m.group(1), body))
    elif section == 'data':
        # 0     'a'  (1 byte)      STR:  .asciz "abc"
        m = re.match(r"(\d+)\s+('(?:\\0|.)'|-?\d+)\s*\((\d+)\s*bytes?\)", s)
        if m:
            out.append('DATA %s %d %s' % (m.group(1), char_to_int(m.group(2)), m.group(3)))
    elif section == 'sym':
        m = re.match(r'(\w+)\s+(-?\d+)\s+(S_\w+)', s)
        if m:
            out.append('SYM %s %s %s' % m.groups())

print('\n'.join(out))
PYEOF
}

if [ "$MODE" = "diff" ]; then
    base=$(basename "$AS_FILE" .as)
    EXP="Tests/expected/$base.expected.txt"
    if [ ! -f "$EXP" ]; then
        echo "no expected file at $EXP" >&2
        exit 2
    fi

    canon_actual   | sort > "$WORK/a.txt"
    canon_expected "$EXP" | sort > "$WORK/e.txt"

    echo "comparing $AS_FILE against $EXP"
    echo "  '<' = expected but missing     '>' = produced but not expected"
    echo "----------------------------------------------------------------"
    if diff "$WORK/e.txt" "$WORK/a.txt" > "$WORK/d.txt"; then
        echo "everything matches."
        exit 0
    fi
    grep -E '^[<>]' "$WORK/d.txt"
    echo "----------------------------------------------------------------"
    printf "%d expected fact(s) missing, %d unexpected\n" \
        "$(grep -c '^<' "$WORK/d.txt")" "$(grep -c '^>' "$WORK/d.txt")"
    exit 1
fi

render
