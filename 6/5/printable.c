#include <stdio.h>
#include <ctype.h>
int strip = 0;  /* => discard special characters */
int invert = 0; /* => if any printable characters are contained, the name is not
                   printed */

int is_printable(FILE *fp);

/*
 * if there are no file name argument, it do nothing.
 */

int main(int argc, char *argv[])
{
    FILE *fp;

    while (argc > 1 && argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 's':
                strip = 1;
                break;
            case 'v':
                invert = 1;
                break;
            default:
                fprintf(stderr, "%s: unknown arg %s\n", argv[0], argv[1]);
                return 1;
        }

        argc--;
        argv++;
    }

    if (argc != 1) {
        for (int i = 1; i < argc; i++) {
            if ((fp = fopen(argv[i], "r")) == NULL) {
                fprintf(stderr, "%s: can't open %s\n", argv[0], argv[i]);
                return 1;
            } else {
                if (is_printable(fp)) {
                    printf("%s\n", argv[i]);
                }
                fclose(fp);
            }
        }
    }

    return 0;
}

int is_printable(FILE *fp)
{
    int c;
    int val = 1;

    while ((c = getc(fp)) != EOF) {
        if (isascii(c) && (isprint(c) || c == '\n' || c == '\t' || c == ' ')) {
            //
        } else {
            val = 0;
        }
    }

    return invert ? !val : val;
}
