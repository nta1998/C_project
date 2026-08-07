#!/usr/bin/env python3
"""
Renders the result of first_pass() as the table used in the course spec:

    Address    Source Code            Machine Code (binary)
    (decimal)
    0100       MAIN:  add $3,$5,$9    000000 00011 00101 01001 00001 000000

Reads the field dump produced by Tests/dump_first_pass.sh.
Not meant to be run directly - use dump_first_pass.sh.

Bit layout (32 bit word, opcode in the high bits):
    R:  opcode(31-26) rs(25-21) rt(20-16) rd(15-11) funct(10-6) unused(5-0)
    I:  opcode(31-26) rs(25-21) rt(20-16) immediate(15-0)
    J:  opcode(31-26) reg(25)   address(24-0)
"""

import sys
import re

ATTR = ['code', 'data', 'external']

W_ADDR = 10
W_SRC = 34


def bits(value, width):
    return format(value & ((1 << width) - 1), '0%db' % width)


def machine_binary(hexword):
    """Split a 32 bit word into the fields of its instruction format."""
    w = int(hexword, 16)
    op = (w >> 26) & 0x3F

    if op in (0, 1):                                    # R type
        return ' '.join([bits(op, 6), bits(w >> 21, 5), bits(w >> 16, 5),
                         bits(w >> 11, 5), bits(w >> 6, 5), bits(w, 6)])

    if 10 <= op <= 24:                                  # I type
        # a branch target is a label, so the offset is only known in pass 2
        imm = '?' if 15 <= op <= 18 else bits(w, 16)
        return ' '.join([bits(op, 6), bits(w >> 21, 5), bits(w >> 16, 5), imm])

    if op in (30, 31, 32, 63):                          # J type
        reg = (w >> 25) & 1
        if op == 63 or reg == 1:        # hlt, or the register form of jmp
            addr = bits(w, 25)
        else:                           # label form: resolved in pass 2
            addr = '?'
        return ' '.join([bits(op, 6), str(reg), addr])

    return '%s   <unknown opcode %d>' % (bits(w, 32), op)


def main():
    fields_path, console_path, as_file = sys.argv[1], sys.argv[2], sys.argv[3]

    ret = None
    icf = dcf = 0
    code, data, syms = [], [], []

    for line in open(fields_path, encoding='utf-8', errors='replace'):
        p = line.rstrip('\n').split(' ')
        tag = p[0]
        if tag == 'RET':
            ret = p[1]
        elif tag == 'ICF':
            icf = int(p[1])
        elif tag == 'DCF':
            dcf = int(p[1])
        elif tag == 'CODE':
            code.append((int(p[1]), p[2], ' '.join(p[3:]).strip()))
        elif tag == 'DATA':
            data.append((int(p[1]), int(p[2]), int(p[3])))
        elif tag == 'SYM':
            syms.append((p[1], int(p[2]), int(p[3]), p[4]))

    console = [l.rstrip('\n') for l in
               open(console_path, encoding='utf-8', errors='replace') if l.strip()]
    nerr = sum(1 for l in console if l.startswith('Error'))

    # a data row that lands exactly on a data symbol gets that symbol's name
    label_at = {}
    for name, val, attr, _ in syms:
        if ATTR[attr] == 'data':
            label_at.setdefault(val, name)

    print('=' * 78)
    print('first_pass() on %s' % as_file)
    print('=' * 78)
    print()
    print('Return value: %s      err_found() == %s' %
          (ret, 'TRUE' if nerr else 'FALSE'))
    print('IC_START = 100      ICF = %d      DCF = %d' % (icf, dcf))
    print()

    print('%-*s %-*s %s' % (W_ADDR, 'Address', W_SRC, 'Source Code',
                            'Machine Code (binary)'))
    print('%-*s' % (W_ADDR, '(decimal)'))
    print('%s %s %s' % ('-' * W_ADDR, '-' * W_SRC, '-' * 41))

    if not code and not data:
        print('(both images are empty)')

    for addr, hexword, src in code:
        src = re.sub(r'\s+', ' ', src).strip()
        print('%-*s %-*s %s' % (W_ADDR, '%04d' % addr, W_SRC, src[:W_SRC],
                                machine_binary(hexword)))

    for addr, val, size in data:
        shown = addr + icf                  # after symbol_table_shift_data()
        name = label_at.get(shown, '')
        src = ('%s:' % name) if name else ''
        print('%-*s %-*s %s' % (W_ADDR, '%04d' % shown, W_SRC, src,
                                bits(val, size * 8)))

    print()
    print('Symbol table after the first pass:')
    print()
    print('%-16s %-16s %s' % ('Symbol', 'Value (decimal)', 'Attributes'))
    print('%s %s %s' % ('-' * 16, '-' * 16, '-' * 12))
    if not syms:
        print('(empty)')
    for name, val, attr, is_entry in syms:
        extra = ', entry' if is_entry == 'TRUE' else ''
        print('%-16s %-16d %s%s' % (name, val, ATTR[attr], extra))

    print()
    print('Console output:')
    if console:
        for l in console:
            print('  ' + l)
    else:
        print('  (nothing was printed)')


if __name__ == '__main__':
    main()
