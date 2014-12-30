#include <stdio.h>
#include <ctype.h>

/***
 * "sed -n l" is simpler than this answer because it has two if-branches in
 * order to verify whether an input character is '\n'.
 ***/

int main()
{
	int c;

	while ((c = getchar()) != EOF) {
		if (isascii(c) &&
				(isprint(c) || c == '\n' || c == '\t' || c == ' ')) {
			if (c == '\t') {
				printf("\\t");
			} else if (c == '\\') {
				printf("\\\\");
			} else {
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
					putchar('$');

				}

				putchar(c);
			}
		} else {
			printf("\\%03o", c);
		}
	}

	return 0;
}
