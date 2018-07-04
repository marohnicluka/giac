#ifndef NAUTYWRAPPER_H
#define NAUTYWRAPPER_H

#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* return nonzero if graphs represented by the sequences adj1
 * and adj2 of adjacency lists, else return zero */
int nautywrapper_is_isomorphic(int isdir,int n,int *adj1,int *adj2,int *sigma);

/* write the generators of Aut(G), where G is represented by the sequence
 * adj of adjacency lists, to the temporary file f and returns length of f
 * (leaves the file open, it should be closed afterwards) */
void nautywrapper_aut_generators(int isdir,int n,int *adj,FILE *f);

/* compute the canonical labeling for the graph represented by the sequence
 * adj of adjacency lists */
void nautywrapper_canonical(int isdir,int n,int *adj,int *clab);

#if defined(__cplusplus)
}
#endif

#endif // NAUTYWRAPPER_H
