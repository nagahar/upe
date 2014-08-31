#include <stdio.h>
#include <unistd.h>

int main()
{
	char *const args[] = {
		"/bin/echo",
		"abc",
		"def",
		NULL, //C99では末尾にカンマがあってもOK！
	};

	execvp(args[0], args);

	//制御が戻ってくるのは、エラーが発生した場合のみ
	perror(args[0]);
	return -1;
}

