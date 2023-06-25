/*
//	@(#) vec.h
*/
# if	!defined( VEC_H)
# define	VEC_H
//
// define elt_t on the compile command line -Delt_t="uint8_t"
//
# include	<stdlib.h>
# include	<stdint.h>

typedef	struct	vec_t	vec_t;

result_t	vec_Create (vec_t** tsp, size_t size);
result_t	vec_grow (vec_t* ts, size_t size);
result_t	vec_Delete (vec_t** tsp);

result_t	vec_append (vec_t* ts, elt_t pos);
// append: vec [used++]	== pos

result_t	vec_insert (vec_t* ts, size_t i, elt_t pos);
// insert:
//   i>=used ==> error: E_OUTSIDE
//   vec [i>j>=0] == vec'[j] && vec [i] == pos && vec [used>=j>i] == vec'[j-1]
//   used == used' + 1

result_t	vec_put (vec_t* ts, size_t i, elt_t pos);
// put:
//   i < used ==> error: E_AGAIN
//   vec [i] == pos && used == i + 1

result_t	vec_replace (vec_t* ts, size_t i, elt_t pos);
// replace:
//  i>=used ==> error: E_OUTSIDE
//  vec [i] == pos
//  i >= used'	=> used == i + 1 [put(i,pos)]

result_t	vec_clear (vec_t* ts, size_t i);
// clear:
//  i>=used ==> error: E_OUTSIDE
//  i < used => vec [i] == 0 

result_t	vec_remove (vec_t* ts, size_t i);
// remove:
//  i>=used ==> error: E_OUTSIDE
//  vec [0<=j<i] == vec'[j] && vec [i<=j<used-1] == vec'[j+1]
//  used == used'-1

result_t	vec_get (vec_t* ts, size_t i, elt_t* pos);
// get:
//  i>=used ==> error: E_OUTSIDE
//  pos == vec [i]

size_t		vec_used (vec_t* ts);
size_t		vec_size (vec_t* ts);

# endif
