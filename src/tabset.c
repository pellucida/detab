/*
// @(#) tabset.c
*/

# include	<stdio.h>
# include	<stdint.h>
# include	<string.h>
# include	<stdlib.h>
# include	<stdarg.h>
# include	<stdint.h>
# include	<errno.h>

# include	"utils.h"
# include	"errors.h"
# include	"new.h"
# include	"vec.h"
# include	"tabset.h"

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
pos_t	tabs_repeat (pos_t pos, pos_t repeat) {
	pos_t	result	= ((pos/repeat)+1)*repeat;
	return	result;
}
	
pos_t	tabset_next (tabset_t* ts, pos_t pos) {
	pos_t	result	= 0;
	pos_t	maxshift	= (ts->shifts)?vec_used (ts->shifts):0;
	if (pos < maxshift) {
		delta_t	delta	= 0;
		vec_get (ts->shifts, pos, &delta);
		result	= pos + delta;
		
	}
	else	{
		result	= tabs_repeat (pos, ts->repeat);
	}
	return	result;
}

static	inline	result_t	get_number (pos_t* pos, char* str, char** nextp){
	unsigned long	p	= 0;
	result_t	result	= get_positive (&p, str, nextp);
	if (result==ok) {
		*pos	= p;
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
			result	= -E__(NOT_ASCENDING);
			tabs	= 0;
		}
		else {
			size_t	i	= previous;
			for (i=previous; i < pos && result==ok; ++i) {
				pos_t	delta	= (pos - i);
				if (delta <= UINT8_MAX) {
					result	= vec_put (shifts, i, (delta_t)delta);
				}
				else	{
					result	= -E__(NUMBER_TOO_BIG);
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
				result	= -E__(NOT_POSITIVE);
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
