/*
//	@(#) errors.h
*/
# if	!defined( ERRORS_H)
# define	ERRORS_H
# include	<errno.h>
# include	<string.h>

enum	{
	DETAB_MASK	= 0x10000,
};
typedef	int	result_t;
# define	E_Code( code, mask)	E_##code	= ((n_##code)|(mask))

enum	{
	n_NOT_MONO_ASCENDING	= 1,
	n_NOT_POSITIVE		= 2,
	n_OUTSIDE		= 3, 
	n_AGAIN			= 4, 
	n_NUMBER_TOO_BIG	= 5,
};
enum	{
	err	= -1,
	ok	= 0,

	E_Code( NOT_MONO_ASCENDING, DETAB_MASK),
	E_Code( NOT_POSITIVE, DETAB_MASK),
	E_Code( OUTSIDE, DETAB_MASK),
	E_Code( AGAIN, DETAB_MASK),
	E_Code( NUMBER_TOO_BIG, DETAB_MASK),
};

static	char* 	detab_error_messages []	= {
	[ok]	= "No error",
	[n_NOT_MONO_ASCENDING]	= "Tab position must be strictly ascending",
	[n_NOT_POSITIVE]	= "Positive number expected",
	[n_OUTSIDE]		= "Access outside range",
	[n_NUMBER_TOO_BIG]	= "Tab stop too far from previous stop", 
	[n_AGAIN]		= "Cannot put() into existing location. User replace()",
};

static	inline	char*	error_msg (result_t r) {
	char*	result	= 0;
	if ((r & DETAB_MASK) == DETAB_MASK) {
		int	en	= r & ~DETAB_MASK;
		result	= detab_error_messages [en];
	}
	else	{
		result	= strerror (r);
	}
	return	result;
}
# endif
