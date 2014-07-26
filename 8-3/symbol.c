#include "hoc.h"
#include "y.tab.h"

static Symbol *symlist = 0;

Symbol *lookup(char *s)
{
	Symbol *sp;

	for (sp = symlist; sp != (Symbol *) 0; sp = sp->next)
	{
		if (strcmp(sp->name, s) == 0) {
			return sp;
		}
	}
	
	return 0;
}


Symbol *install(char *s, int t, double d)
{
	Symbol *sp;
	char *emalloc();

	sp = (Symbol *) emalloc(sizeof(Symbol));
	sp->name = emalloc(strlen(s) + 1); /* +1 for '\0' */
	strcpy(sp->name, s);
	sp->type = t;
	sp->u.val = d;
	sp->next = symlist;
	symlist = sp;
	return sp;
}


char *emalloc(unsigned n)
{
	char *p;

	p = malloc(n);
	if (p == 0) {
		execerror("out of memory", (char *) 0);
	}

	return p;
}
