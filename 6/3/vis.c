#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define	OPT_LEN	2

/*
 * Argument of vis is better than separate program because it is difficult to
 * justify executable programs from file name.
 */

void putch(int c, int strip);

int main(int argc, char *argv[])
{
	int c, strip = 0;
	int val = 0;

	if (argc > 1 && strncmp(argv[1], "-s", OPT_LEN) == 0) {
		strip = 1;
		unsigned long n;
		n = strlen(argv[1]) - OPT_LEN;
		if (0 < n) {
			char *num_s = NULL;
			num_s = malloc(n + 1);
			strncpy(num_s, argv[1] + OPT_LEN, n);
			val = atoi(num_s);
			free(num_s);
		}
	}

	int *buf = NULL;
	if (0 < val) {
		buf = malloc((unsigned long)val);
	}

	int counter = 0;
	while ((c = getchar()) != EOF) {
		if (0 < val) {
			counter++;
			if (counter < val) {
				buf[counter - 1] = c;
			} else {
				for (int i = 0; i < val; i++) {
					putch(buf[i], strip);
				}

				putch(c, strip);
				val = 0;
			}
		} else {
			putch(c, strip);
		}
	}

	if (0 < val) {
		free(buf);
	}

	return 0;
}

void putch(int c, int strip)
{
	if (isascii(c) &&
			(isprint(c) || c == '\n' || c == '\t' || c == ' ')) {
		putchar(c);
	} else if (!strip) {
		printf("\\%03o", c);
	}
}

