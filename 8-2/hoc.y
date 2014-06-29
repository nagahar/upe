%{
double	mem[26]; /* 変数用メモリ */
%}
%union {	/* stackの型 */
	double	val;	/* 値の場合 */
	int	index;	/* memのインデックスの場合 */
}
%token	<val>	NUMBER
%token	<index>	VAR
%type	<val>	expr /* 非終端記号はtypeでグループ化する */
%right	'='
%left	'+' '-' /* 左結合 */
%left	'*' '/' '%'  /* より優先度の高い左結合 */
%left	UNARYMINUS UNARYPLUS
%%
list:	/* 空白も対象 */
	| list '\n'
	| list expr '\n' { printf("\t%.8g\n", $2); /* 変数pに以前の結果を保持する */ mem['p' - 'a'] = $2; }
	| list expr ';' { printf("\t%.8g\n", $2); /* 変数pに以前の結果を保持する */ mem['p' - 'a'] = $2; }
	| list error '\n' { yyerrok; }
	;
expr:	NUMBER
	| VAR { $$ = mem[$1]; }
	| VAR '=' expr { $$ = mem[$1] = $3; }
	| '-' expr %prec UNARYMINUS { $$ = -$2; }
	| '+' expr %prec UNARYPLUS { $$ = $2; }
	| expr '%' expr { $$ = fmod($1, $3); }
	| expr '+' expr { $$ = $1 + $3; }
	| expr '-' expr { $$ = $1 - $3; }
	| expr '*' expr { $$ = $1 * $3; }
	| expr '/' expr {
		if ($3 == 0.0) {
			execerror("division by zero", "");
		}
		$$ = $1 / $3; }
	| '(' expr ')'  { $$ = $2; }
	;
%% /* 文法規則の終了 */
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <setjmp.h>

char *progname;
int lineno = 1;
jmp_buf begin;

int main(int argc, char *argv[])
{
	int fpecatch();

	progname = argv[0];
	setjmp(begin);
	signal(SIGFPE, fpecatch);
	yyparse();
	return 0;
}


int execerror(const char *s, const char *t)
{
	/* 実行時のエラーから回復する */
	warning(s, t);
	longjmp(begin, 0);
	return 0;
}


int fpecatch()
{
	/* 浮動小数点の桁あふれを捕捉する */
	return execerror("floating point exception", (char *)0);
}


int yylex()
{
	int c;
	while ((c = getchar()) == ' ' || c == '\t')
		;
	if (c == EOF) {
		return 0;
	}

	if (c == '.' || isdigit(c)) {
		/* number */
		ungetc(c, stdin);
		scanf("%lf", &yylval.val);
		return NUMBER;
	}

	if (islower(c)) {
		yylval.index = c - 'a'; /* ascii のみのため */
		return VAR;
	}


	if (c == '\n') {
		lineno++;
	}

	return c;
}


int yyerror(const char *s)
{
	/* 文法エラーのとき */
	warning(s, (char* )0);
	return 0;
}


int warning(const char *s, const char *t)
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t){
		fprintf(stderr, " %s", t);
	}

	fprintf(stderr, " near line %d\n", lineno);
	return 0;
}

