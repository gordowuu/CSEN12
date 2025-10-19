/*
 * Gordon Wu
 * CSEN12
 * 9/29/2025
 *
 * unsorted.c
 * Implementation of an unordered set of strings.
 *
 * Big O Time Complexities:
 * createSet: O(n)
 * destroySet: O(n)
 * numElements: O(1)
 * addElement: O(n) 
 * removeElement: O(n)
 * findElement: O(n)
 * getElements: O(n)
 * search: O(n)
 */

#include <stdlib.h>
#include <stdio.h>
#include "set.h"
#include <string.h>
#include <assert.h>

typedef struct set {
	size_t count;
	size_t length;
	char **data;
} SET;

/* private helper: linear search for elt; returns index or -1 if not found
 * Big O: O(n)
 */
static int search(SET *sp, char *elt);

/* createSet: allocate and initialize a new SET holding up to
 * maxElts elements.
 * Big O: O(n)
 */
SET *createSet(int maxElts) {
	SET *sp;
	sp = malloc(sizeof(SET));
	assert(sp != NULL);
	assert(maxElts > 0);
	sp->count = 0;
	sp->length = maxElts;
	sp->data = malloc(sizeof(char*) * maxElts);
	assert(sp->data != NULL);
	return sp;
}

/* destroySet: free all strings owned by the set, the array, and the set
 * structure.
 * Big O: O(n)
 */
void destroySet(SET *sp) {
	assert(sp != NULL);
	for (int i = 0; i < sp->count; i++) {
		free(sp->data[i]);
	}
	free(sp->data);
	free(sp);
}

/* numElements: return the number of elements stored
 * Big O: O(1)
 */
int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->count;
}

/* addElement: if elt is not present and there is space, insert a copy of
 * elt into the set.
 * Big O: O(n) for linear search ; insertion is O(1).
 */
void addElement(SET *sp, char *elt) {
	assert((elt != NULL) && (sp != NULL));
	assert(sp->count < sp->length);
	if (search(sp, elt) == -1) {
		char* newElt = strdup(elt);
		assert(newElt != NULL);
		sp->data[sp->count] = newElt;
		sp->count++;
	}
}

/* removeElement: remove elt if present. the last element is then moved into the removed slot.
 * Big O: O(n) for linear search; removal is O(1).
 */
void removeElement(SET *sp, char *elt) {
	assert((sp != NULL) && (elt != NULL));
	int idx = search(sp, elt);
	if (idx != -1) {
		free(sp->data[idx]);
		sp->data[idx] = sp->data[sp->count-1];
		sp->count--;
	}
}

/* findElement: return pointer to stored string equal to elt, or NULL if not
 * found.
 * Big O: O(n)
 */
char *findElement(SET *sp, char *elt) {
	assert((sp != NULL) && (elt != NULL));
	int idx = search(sp, elt);
	if (idx == -1) {
		return NULL;
	}
	return sp->data[idx];
}

/* getElements: return a newly-allocated array of pointers to the elements stored in the set.
* Big O: O(n)
*/
char **getElements(SET *sp) {
	assert(sp != NULL);
	char **cpy;
	cpy = malloc(sp->count*sizeof(char *));
	assert(cpy != NULL);
	memcpy(cpy, sp->data, sp->count * sizeof(char *)); // shallow copies pointers with memcpy
	return cpy;
}

static int search(SET *sp, char *elt) {
	assert(sp != NULL);
	for (int i = 0; i < sp->count; i++) {
		if (strcmp(sp->data[i], elt) == 0) {
			return i;
		}
	}
	return -1;
}
