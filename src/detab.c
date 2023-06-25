/*
// @(#) detab.c
//
*/

# include	<unistd.h>
# include	<stdio.h>
# include	<stdint.h>
# include	<string.h>
# include	<stdlib.h>
# include	<stdarg.h>
# include	<stdint.h>
# include	<errno.h>

# include	"errors.h"
# include	"new.h"
# include	"vec.h"


static	char*	prog	= "detab";

static	void	Usage (){
	fprintf (stderr, "Usage: %s [-T tab_repeat] [-t n,...] [file]\n", prog);
	fprintf (stderr, "\tThe default is %s -T 8\n", prog);
	fprintf (stderr, "\tTabs set at 3,6,9 then 14,21,... %s -t 3,6,9 -T 7\n", prog);
	exit (EXIT_FAILURE);
}

static	void	fatal (char* fmt, ...) {
	va_list	ap;
	va_start (ap, fmt);
	fprintf (stderr, "ERROR: %s ", prog);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	exit (EXIT_FAILURE);
}
static	void	fatal_error (char* msg, int errn) {
	fatal ("%s %s\n", msg, error_msg (-errn));
}


enum	{
	TAB_REPEAT	= 8,
};
typedef	uint16_t 	pos_t;
typedef	elt_t	delta_t;
typedef	struct	tabset_t	tabset_t;
struct	tabset_t	{
	vec_t*	shifts;
	pos_t	repeat;
};

result_t	tabset_Create (tabset_t** tsp, vec_t* shifts, pos_t repeat) {
	tabset_t*	ts	= 0;
	result_t	result	= New (ts);
	if (result==ok) {
		*ts	= (typeof(*ts)){
			.repeat	= repeat,
			.shifts	= shifts,
		};
		*tsp	= ts;
		result	= ok;
	}
	return	result;
}
// --------------------------------------------------
// convert 'str' to into positive number [0..n]]
//
result_t	get_number (pos_t* tp, char* str, char** nextp) {
	result_t	result	= -E_NOT_POSITIVE;
	char*	tend	= 0;
	pos_t	t	= strtoul (str, &tend, 10);
	if (str < tend){
		*tp	= t;
		result	= ok;
	}
	if (nextp) {
		*nextp	= tend;
	}
	return	result;
}
	
// _next: pos -> next tab stop

pos_t	tabset_next (tabset_t* ts, pos_t pos) {
	pos_t	result	= 0;
	pos_t	maxshift	= (ts->shifts)?vec_used (ts->shifts):0;
	if (pos < maxshift) {
		delta_t	delta	= 0;
		vec_get (ts->shifts, pos, &delta);
		result	= pos + delta;
		
	}
	else	{
		result	= ((pos/ts->repeat)+1)*ts->repeat;
	}
	return	result;
}

// -t tabs -- tabs : num | tabs ',' num
enum	{
	TABLIST_SEP	= ',',
};
result_t	tabs_parse (vec_t* shifts, char* tabstr) {
	result_t	result	= ok;
	char*	tabs	= tabstr;
	pos_t	previous	= 0;
	while (tabs) {
		char*	next	= 0;
		pos_t	pos	= 0; 
		result	= get_number (&pos, tabs, &next);
		if (result!=ok) {
			tabs	= 0;
		}
		else if (previous >= pos) {
			result	= -E_NOT_MONO_ASCENDING;
			tabs	= 0;
		}
		else {
			size_t	i	= previous;
			for (i=previous; i < pos && result==ok; ++i) {
				pos_t	delta	= (pos - i);
				if (delta < 256) {
					result	= vec_put (shifts, i, (delta_t)delta);
				}
				else	{
					result	= -E_NUMBER_TOO_BIG;
					tabs	= 0;
				}
			}
			if (result != ok || *next == '\0') {
				tabs	= 0;
			}
			else if (*next == TABLIST_SEP) {
				tabs	= next+1;
				previous	= pos;
			}
			else {
				tabs	= 0;
				result	= -E_NOT_POSITIVE;
			}
		}
	}
	return	result;
}
result_t	tabset_Make (tabset_t** tsp, char* tabstr, char* repeatstr) {
	tabset_t*	ts	= 0;
	pos_t	repeat	= TAB_REPEAT;
	vec_t*	shifts	= 0;
	result_t	result	= vec_Create (&shifts, 0);
	
	if (result!=ok) {
		fatal_error ("vec_Create (shifts)", result);
	}
	if (tabstr) {
		result	= tabs_parse (shifts, tabstr);
	}
	if (result != ok) {
		fatal_error ("-t <n>[,<m>...]", result);
	}
	if (repeatstr) {
		result	= get_number (&repeat, repeatstr, 0);
	}
	if (result != ok) {
		fatal_error ("-T <n>", result);
	}
	result	= tabset_Create (&ts, shifts, repeat);
	if (result == ok) {
		*tsp	= ts;
	}
	return	result;
}

static	inline	void	spaces (FILE* output, size_t n){
	int	i	= 0;
	for (i=0; i < n; ++i) {
		fputc (' ', output);
	}
}
static	char*	basename (char* s) {
	char*	result	= s;
	char*	t	= strrchr (s, '/');
	if (t) {
		result	= t+1;
	}
	return	result;
}
int	main (int argc, char* argv[]) {
	tabset_t*	tabset	= 0;
	result_t	result	= 0;
	int	opt	= EOF;
	char*	tabs	= 0;
	FILE*	input	= stdin;
	FILE*	output	= stdout;
	int	ch	= EOF;
	pos_t	p	= 0;
	char*	repeat	= 0;

	prog	= basename (argv[0]);


	while ((opt = getopt (argc, argv, "ht:T:"))!=EOF) {
		switch (opt) {
		case	't':
			tabs	= optarg;
		break;
		case	'T':
			repeat	= optarg;
		break;
		case 	'h':
		default:
			Usage ();
		break;
		}
	}
	result	= tabset_Make (&tabset, tabs, repeat);
	if (result != ok) {
		fatal_error ("tabset_Make", result);
	}
	if (optind+1 == argc) {
		char*	infile	= argv [optind];
		input	= fopen (infile, "r");
		if (input==0) {
			fatal ("cannot open '%s'\n", infile);
		}
	}
	while ((ch = fgetc (input)) != EOF) {
		if (ch == '\n') {
			fputc (ch, output);
			p	= 0;
		}
		else if (ch == '\t') {
			pos_t	pos	= tabset_next (tabset, p);
			if (pos > p) {
				spaces (output, pos-p);
				p	= pos;
			}
		}
		else {
			fputc (ch, output);
			++p;
		}
	}
	exit (EXIT_SUCCESS);
}
