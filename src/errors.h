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
# define	E__( code)	E_##code
# define	E_Code( code, mask)	E__(code)	= ((code)|(mask))

enum	{
	NOT_ASCENDING	= 1,
	NOT_POSITIVE	= 2,
	NOT_INITIALIZED	= 3, 
	WOULD_OVERWRITE	= 4, 
	NUMBER_TOO_BIG	= 5,
};
enum	{
	err	= -1,
	ok	= 0,

	E_Code( NOT_ASCENDING, DETAB_MASK),
	E_Code( NOT_POSITIVE, DETAB_MASK),
	E_Code( NOT_INITIALIZED, DETAB_MASK),
	E_Code( WOULD_OVERWRITE, DETAB_MASK),
	E_Code( NUMBER_TOO_BIG, DETAB_MASK),
};

static	char* 	detab_error_messages []	= {
	[ok]	= "No error",
	[NOT_ASCENDING]	= "Tab position must be strictly ascending",
	[NOT_POSITIVE]	= "Positive number expected",
	[NOT_INITIALIZED]	= "Access outside range",
	[NUMBER_TOO_BIG]	= "Tab stop too far from previous stop", 
	[WOULD_OVERWRITE]	= "Cannot put() into existing location. Use eplace()",
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
