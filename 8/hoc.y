%{
#define	YYSTYPE double
%}
%token	NUMBER
%left	'+' '-' /* 左結合 */
%left	'*' '/' '%'  /* より優先度の高い左結合 */
%left	UNARYMINUS UNARYPLUS	/* new */
%%
list:	/* 空白も対象 */
	| list '\n'
	| list expr '\n' { printf("\t%.8g\n", $2); }
expr:	NUMBER { $$ = $1; }
	| '-' expr %prec UNARYMINUS { $$ = -$2; } /* new */
	| '+' expr %prec UNARYPLUS { $$ = $2; } /* new */
	| expr '%' expr { $$ = fmod($1, $3); } /* new */
	| expr '+' expr { $$ = $1 + $3; }
	| expr '-' expr { $$ = $1 - $3; }
	| expr '*' expr { $$ = $1 * $3; }
	| expr '/' expr { $$ = $1 / $3; }
	| '(' expr ')'  { $$ = $2; }
%% /* 文法規則の終了 */
#include <stdio.h>
#include <ctype.h>
#include <math.h>

char *progname;
int lineno = 1;

int main(int argc, char *argv[])
{
	progname = argv[0];
	yyparse();
	return 0;
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
		scanf("%lf", &yylval);
		return NUMBER;
	}

	if (c == '\n') {
		lineno++;
	}

	return c;
}

/* 型を一致させないとyacc本体とのname conflictを起こす */
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
