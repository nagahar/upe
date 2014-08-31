%{
#include "hoc.h"
extern double Pow();
%}
%union {	/* stackの型 */
	double	val;	/* 値の場合 */
	Symbol	*sym;	/* Symbolテーブルのポインタの場合 */
	double	*vals;
	char	**strs;
}
%token	<val>	NUMBER
%token	<sym>	VAR BLTIN UNDEF CNST
%type	<val>	expr asgn exec	/* 非終端記号はtypeでグループ化する */
%type	<vals>	tpl
%type	<strs>	stpl
%right	'='
%left	'+' '-' /* 左結合 */
%left	'*' '/' '%'  /* より優先度の高い左結合 */
%left	UNARYMINUS UNARYPLUS
%left	'!'
%right	'^'	/* べき乗 */
%%
list:	/* 空白も対象 */
	| list '\n'
	| list asgn '\n'
	| list expr '\n' { printf("\t%.8g\n", $2); }
	| list expr ';' { printf("\t%.8g\n", $2); }
	| list exec '\n' { printf("\tdone!\n"); }
	| list error '\n' { yyerrok; }
	;
asgn:	VAR '=' expr { $$ = $1->u.val = $3; $1->type = VAR; }
	| CNST '=' expr { execerror("constant value", $1->name); }
	;
exec:	'!' stpl { $$ = exec($2); }
	;
stpl:	
	| VAR { $$ = mknlst($1->name); }
	| stpl ',' VAR { $$ = addnlst($1, $3->name); }
	;
tpl:	
	| expr { $$ = mkvlst($1); }
	| tpl ',' expr { $$ = addvlst($1, $3); }
	;
expr:	NUMBER
	| VAR { if ($1->type == UNDEF) {
			execerror("undefined variable", $1->name);
		}
		$$ = $1->u.val; }
	| CNST { $$ = $1->u.val; }
	| asgn
	| BLTIN '(' tpl ')' { $$ = (*($1->u.ptr))($3); }
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
	| expr '^' expr { $$ = Pow($1, $3); }
	;
%% /* 文法規則の終了 */
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>

#define MAX_SIZE 100

char *progname;
int lineno = 1;
jmp_buf begin;
static double vals[MAX_SIZE];
static char *strings[MAX_SIZE];
static int tail;

int main(int argc, char *argv[])
{
	int fpecatch();

	progname = argv[0];
	init();
	setjmp(begin);
	signal(SIGFPE, fpecatch);
	yyparse();
	return 0;
}

double *addvlst(double *args, double a)
{
	if (tail + 1 < MAX_SIZE) {
		tail++;
		args[tail] = a;
	} else {
		execerror("out of range argument", (char *)0);
	}

	return args;
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


double *mkvlst(double a)
{
	tail = 0;
	vals[0] = a;
	return vals;
}


char **mknlst(char *a)
{
	fprintf(stderr, "!!!!!");
	fprintf(stderr, "%s\n", a);
	tail = 0;
	strings[tail] = a;
	strings[tail + 1] = NULL;
	return strings;
}


char **addnlst(char **args, char *a)
{
	fprintf(stderr, "@@@@%s\n", a);
	if (tail + 1 < MAX_SIZE - 1) {
		tail++;
		args[tail] = a;
		args[tail + 1] = NULL;
	} else {
		execerror("out of range argument", (char *)0);
	}

	return args;
}


double exec(char **args)
{
	int status;
	int w, pid;
	if ((pid = fork()) == 0) {
		if (-1 == execvp(args[0], args)) {
			perror("couldn't execute");
		}
	}

	while ((w = wait(&status)) != pid && w != -1)
		;

	return 0.0;
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

	if (isalpha(c)) {
		Symbol *s;
		char sbuf[100], *p = sbuf;
		do {
			*p++ = c;
		} while ((c = getchar()) != EOF && isalnum(c));
		ungetc(c, stdin);
		*p = '\0';
		if ((s = lookup(sbuf)) == 0) {
			s = install(sbuf, UNDEF, 0.0);
		}

		yylval.sym = s;
		return s->type == UNDEF ? VAR : s->type;
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

