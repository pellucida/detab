/*
// @(#) utils.c
*/

# include	<stdio.h>
# include	<string.h>
# include	<stdlib.h>
# include	<stdarg.h>
# include	<errno.h>

# include	"errors.h"
# include	"utils.h"


void	fatal (char* fmt, ...) {
	va_list	ap;
	va_start (ap, fmt);
	fprintf (stderr, "ERROR: %s ", progname());
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	exit (EXIT_FAILURE);
}
void	fatal_error (char* msg, int errn) {
	fatal ("%s %s\n", msg, error_msg (-errn));
}

void	spaces (FILE* output, size_t n){
	int	i	= 0;
	for (i=0; i < n; ++i) {
		fputc (' ', output);
	}
}
char*	basename (char* s) {
	char*	result	= s;
	char*	t	= strrchr (s, '/');
	if (t) {
		result	= t+1;
	}
	return	result;
}
char*	progname_util (int what, char* argv0, char* fallback) {
	static	char*	progname	= 0; 
	char*	result	= progname;
	if (what == PROGNAME_SET) {
		if (argv0) {
			progname	= basename (argv0);
		}
		else	{
			progname	= fallback;
		}
	}
	return	result;
}
// --------------------------------------------------
// convert 'str' to into positive number [0..n]]
//
result_t	get_positive (unsigned long* tp, char* str, char** nextp) {
	result_t	result	= -E__(NOT_POSITIVE);
	char*	tend	= 0;
	typeof(*tp)	t	= strtoul (str, &tend, 10);
	if (str < tend){
		*tp	= t;
		result	= ok;
	}
	if (nextp) {
		*nextp	= tend;
	}
	return	result;
}
