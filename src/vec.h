/*
//	@(#) vec.h
*/
# if	!defined( VEC_H)
# define	VEC_H

# include	<stdlib.h>

typedef	struct	vec_t	vec_t;

typedef	unsigned	int	pos_t;

result_t	vec_Create (vec_t** tsp, size_t size);
result_t	vec_grow (vec_t* ts, size_t size);
result_t	vec_Delete (vec_t** tsp);
result_t	vec_append (vec_t* ts, pos_t pos);
result_t	vec_insert (vec_t* ts, size_t i, pos_t pos);
result_t	vec_put (vec_t* ts, size_t i, pos_t pos);
result_t	vec_replace (vec_t* ts, size_t i, pos_t pos);
result_t	vec_clear (vec_t* ts, size_t i);
result_t	vec_remove (vec_t* ts, size_t i);
result_t	vec_get (vec_t* ts, size_t i, pos_t* pos);

size_t		vec_used (vec_t* ts);
size_t		vec_size (vec_t* ts);

# endif
