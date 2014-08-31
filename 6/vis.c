#include <stdio.h>
#include <ctype.h>

int main()
{
	int c;

	while ((c = getchar()) != EOF) {
		if (isascii(c) &&
			(isprint(c) || c == '\n' || c == '\t' || c == ' ')) {
			if (c == ' ') {
				int d = 1;
				while ((c = getchar()) == ' ') {
					d++;
				}

				for (int i = 0; i < d; i++) {
					if (c == '\n') {
						putchar('~');
					} else {
						putchar(' ');
					}
				}
			}

			if (c == '\n') {
				printf("\\n");
			} else if (c == '\t') {
				printf("\\t");
			} else if (c == '\\') {
				printf("\\\\");
			} else {
				putchar(c);
			}
		} else {
			printf("\\%03o", c);
		}
	}

	return 0;
}
