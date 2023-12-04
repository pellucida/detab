/*
// @(#) entab.c
//
// -F	- tabstops indicate fixed field boundaries 
//      - remove any trailing space and insert a tab '\t'
// -Z	- use tabs for space compression
//	- but don't replace single space with a tab
//
// How do you treat tabs in the input?
// [d] - ignore ie delete from input
// [p] - pass through as a normal character ie occupied 1 char space
// [s] - map to a single space
// [i] - insert sufficient spaces to tab to the next -T 8 tabstop
// [h] - honour the tab and absorb preceeding spaces and move to next tabstop
//
// Leading spaces on fields? Suppress?
// Trailing spaces or tabs before newline? Suppress?
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
# include	"utils.h"
# include	"tabset.h"


static	void	Usage (){
	char*	prog	= progname ();
	fprintf (stderr, "Usage: %s [-E (d|h|i|s|p)] [-F|-Z] [-T tab_repeat] [-t n,...] [file]\n", prog);
	fprintf (stderr, "\tThe default is %s -E h -T 8\n", prog);
	fprintf (stderr, "\tTabs set at 3,6,9 then 14,21,... %s -t 3,6,9 -T 7\n", prog);
	exit (EXIT_FAILURE);
}

static	const	char	embedded_tab_options[]	= "dhops";

int	main (int argc, char* argv[]) {
	result_t	result	= 0;
	int	opt	= EOF;
	int	F_flag	= 0;	// treat input as fixed field space padded
	int	Z_flag	= 0;	// only output tab if replacing 2 or more spaces
	int	E_flag	= 0;	// tab handling request
	int	E_opt	= 'h';

	tabset_t*	tabset	= 0;
	char*	tabs	= 0;
	char*	repeat	= 0;

	FILE*	input	= stdin;
	FILE*	output	= stdout;

	int	ch	= EOF;
	pos_t	pos	= 0;	// position in output line
	pos_t	tabstop	= 0;	// position of the next tabstop 
	int	sp	= 0;	// length of run of spaces


	progname_set (argv[0], "entab");

	while ((opt = getopt (argc, argv, "E:ZFht:T:"))!=EOF) {
		switch (opt) {
		case	't':
			tabs	= optarg;
		break;
		case	'T':
			repeat	= optarg;
		break;
		case	'F':
			if (F_flag++ || Z_flag)
				Usage ();
		break;
		case	'Z':
			if (Z_flag++ || F_flag)
				Usage ();
		break;
		case	'E':
			if (E_flag++)
				Usage ();
			E_opt	= optarg [0];
		break;
		case 	'h':
		default:
			Usage ();
		break;
		}
	}
	if (E_flag && strchr (embedded_tab_options, E_opt)==0) {
		fatal_error ("option -E", -E__(UNSUPP_TAB_REQ));
	}
	result	= tabset_Make (&tabset, tabs, repeat);
	if (result != ok) {
		fatal_error ("tabset_Make", result);
	}
	if (optind+1 == argc) {	// only process one file or stdin
		char*	infile	= argv [optind];
		input	= fopen (infile, "r");
		if (input==0) {
			fatal ("cannot open '%s'\n", infile);
		}
	}

	tabstop	= tabset_next (tabset, pos);
	while ((ch = fgetc (input)) != EOF) {
		++pos;
		if (ch == ' ') {
			++sp;
		}
		else if (ch == '\t') {
			if (E_flag)
			switch (E_opt) {
			case	'h':	// Honour tab request
				fputc (ch, output);
				sp	= 0; // any spaces are absorbed
				pos	= tabstop;
				tabstop	= tabset_next (tabset, pos);
			break;
			case	'd':	// Delete tab char
				pos	= pos - 1;
			break;
			case	'p':	// Pass as normal char
				if (sp > 0) {
					spaces (output, sp); // honour spaces
					sp	= 0;
				}
				fputc (ch, output);
			break;
			case	's':	// Map to single space
					++sp;
			break;
			case	'i':	// space to next /8 tab
				{
				pos_t	next	= tabs_repeat (pos, TAB_REPEAT);
				sp	 += next - pos;
				pos	= next;
				}
			break;
			default:
				;
			break;
			}
		}
		else {
			if (ch == '\n') {
				if (sp > 0) {
					spaces (output, sp);
				}
				fputc (ch, output);
				pos	= 0;
				tabstop	= tabset_next (tabset, pos);
			}
			else	{ // ch not space, tab or newline
				if (sp > 0) {
					spaces (output, sp);
				}
				fputc (ch, output);
			}
			sp	= 0;	// non space char always breaks run
		}
		if (pos == tabstop) {
			if (F_flag || sp > 0) {
				if (sp == 1 && Z_flag) {
					spaces (output, sp);
				}
				else	{
					fputc ('\t', output);
				}
				sp	= 0;
			}
			tabstop	= tabset_next (tabset, pos);
		}
	}
	exit (EXIT_SUCCESS);
}
