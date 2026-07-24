#include <stdio.h>
#include "../Headers/parser.h"

int main(int argc, char *argv[])
{
    FILE *f;
    char buffer[82];
    int line_no = 0;
    ParsedLine pl;

    if (argc < 2) {
        printf("usage: %s file.as\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("cannot open %s\n", argv[1]);
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        line_no++;
        if (line_split(buffer, &pl, argv[1], line_no))
            printf("%d: kind=%d label=[%s] name=[%s] rest=[%s]\n",
                   line_no, pl.kind, pl.label, pl.name, pl.rest);
        else
            printf("%d: (rejected)\n", line_no);
    }

    fclose(f);
    return 0;
}