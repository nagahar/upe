typedef struct Symbol {
	char	*name;
	short	type;
	union {
		double	val;
		double	(*ptr)();
	} u;
	struct	Symbol	*next;
} Symbol;

Symbol	*install(), *lookup();

double *mkvlst(double a);
double *addvlst(double *args, double a);

double exec(char **args);
char **mknlst(char *a);
char **addnlst(char **args, char *a);

