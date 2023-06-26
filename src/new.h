/*
//	@(#) new.h
// ---------
// Allocation functions and macros to prevent this error
//	type*	t	= malloc (sizeof(t));
// for	type*	t	= malloc (sizeof(*t));
//
// use	type*	t	= 0;
//	result	= New (t);
*/
# if	!defined( NEW_H)
# define	NEW_H

# include	<stdlib.h>

static	inline	result_t	_New (void* p, size_t size) {
	result_t	result	= -ENOMEM;
	void*	m	= malloc (size);
	if (m) {
		*(void**)(p)	= m;
		result	= ok;
	}
	return	result;
}
static	inline	result_t	_New_Vec (void* v, size_t nelt, size_t size){
	result_t	result	= -ENOMEM;
	void*	vec	= calloc (nelt, size);
	if (vec) {
		*(void**)(v)	= vec;
		result	= ok;
	}
	return	result;
}
static	inline	result_t	_Resize_Vec (void* v, size_t n, size_t size){
	result_t	result	= -ENOMEM;
	void*	old	= *(void**)v;
	if (old==0) {
		result	= _New_Vec (v, n, size);
	}
	else	{
		void*	vec	= realloc (old, n*size);
		if (vec) {
			*(void**)(v)	= vec;
			result	= ok;
		}
	}
	return	result;
}
		
# define	New( p)	(_New (&(p), sizeof(*(p))))
# define	New_Vec( v, n)	(_New_Vec (&(v), (n), sizeof(*v])))
# define	Resize_Vec( v, n)	(_Resize_Vec (&(v), (n), sizeof(*v)))
# endif
