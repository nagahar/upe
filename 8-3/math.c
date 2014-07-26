#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>

extern	int	errno;
double	errcheck();

double Atan2(double *args)
{
	return errcheck(atan(*args / *++args), "atan2");
}

double Log(double *args)
{
	return errcheck(log(*args), "log");
}

double Log10(double *args)
{
	return errcheck(log10(*args), "log10");
}

double Exp(double *args)
{
	return errcheck(exp(*args), "exp");
}

double Sqrt(double *args)
{
	return errcheck(sqrt(*args), "sqrt");
}

double Pow(double x, double y)
{
	return errcheck(pow(x, y), "exponentiation");
}

double Rand()
{
	return errcheck((double)rand() / RAND_MAX, "rand");
}

double integer(double *args)
{
	return (double)(long)*args;
}

double errcheck(double d, char *s)
{
	if (errno == EDOM) {
		errno = 0;
		execerror(s, "arugument out of domain");
	} else if (errno == ERANGE) {
		errno = 0;
		execerror(s, "result out of range");
	}

	return d;
}

