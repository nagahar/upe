#include <stdio.h>
#include <ctype.h>
#define	FOLD_COLUMN	80

/***
 * If the number of printable characters in a line is over 80, it inserts carrige return forcely.
 * Non pritable characters are counted as 1.
 ***/

int count_getchar();
void reset_count();
int count = 0;

int main()
{
	int c;
	while ((c = count_getchar()) != EOF) {
		if (isascii(c) &&
				(isprint(c) || c == '\n' || c == '\t' || c == ' ')) {
			if (c == '\t') {
				printf("\\t");
			} else if (c == '\\') {
				printf("\\\\");
			} else {
				if (c == ' ') {
					int d = 1;
					while ((c = count_getchar()) == ' ') {
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
					reset_count();
				}

				putchar(c);
			}
		} else {
			printf("\\%03o", c);
		}

		if (FOLD_COLUMN == count) {
			printf(">\n");
			reset_count();
		}
	}

	return 0;
}

int count_getchar()
{
	int c = getchar();
	count++;
	return c;
}

void reset_count()
{
	count = 0;
}

