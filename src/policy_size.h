/*
//	@(#) policy_size.h
*/
# if	!defined( POLICY_SIZE_H)
# define	POLICY_SIZE_H

# include	<stdlib.h>

enum	{
	VEC_SIZE_MIN	= 64,
	VEC_SIZE_STEP	= 64,
};
static	inline	size_t	policy_size (size_t size) {
	size_t	result	= VEC_SIZE_MIN;
	if (size > result) {
		result	= ((size + (VEC_SIZE_STEP - 1))/VEC_SIZE_STEP)*VEC_SIZE_STEP;
	}
	return	result;
}
# endif
