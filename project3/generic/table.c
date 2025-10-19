/*
 * Gordon Wu
 * CSEN12
 * 10/19/2025
 *
 * table.c
 * Implementation of a generic hash table for arbitrary data types using 
 * open addressing with linear probing. Elements are stored in an array 
 * with flags indicating Empty, Filled, or Deleted states. Uses function
 * pointers for comparison and hashing to support any data type.
 *
 * Big O Time Complexities (Average Case):
 * createSet: O(m) - where m is maxElts
 * destroySet: O(1) 
 * numElements: O(1)
 * addElement: O(1) average, O(m) worst case
 * removeElement: O(1) average, O(m) worst case
 * findElement: O(1) average, O(m) worst case
 * getElements: O(m) 
 * search: O(1) average, O(m) worst case
 */

#include <stdlib.h>
#include <stdio.h>
#include "set.h"
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef struct set {
	size_t count;
	size_t length;
	void **data;
    char *flags;
    int (*compare)();
    unsigned (*hash)();
} SET;

static int search(SET *sp, void *elt, bool *found);

/* createSet: allocate and initialize a new generic hash table SET holding up to
 * maxElts elements. Stores function pointers for comparison and hashing.
 * Initializes the data array and flags array.
 * Big O: O(m)
 */
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
	SET *sp;
	sp = malloc(sizeof(SET));
	assert(sp != NULL);
	assert(maxElts > 0);
	sp->count = 0;
	sp->length = maxElts;
    sp->compare = compare;
    sp->hash = hash;
	sp->data = malloc(sizeof(void*) * maxElts);
	assert(sp->data != NULL);
	sp->flags = malloc(sizeof(char) * maxElts);
	assert(sp->flags != NULL);
	for (int i = 0; i < maxElts; i++) {
		sp->flags[i] = 'E';
	}
	return sp;
}

/* destroySet: free the data and flags arrays, and the set structure.
 * Does NOT free individual elements as the caller retains ownership of the data.
 * Big O: O(1)
 */
void destroySet(SET *sp) {
	assert(sp != NULL);
	free(sp->data);
	free(sp->flags);
	free(sp);
}

/* numElements: return the number of elements stored
 * Big O: O(1)
 */
int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->count;
}

/* addElement: insert elt in the hash table using linear probing and the
 * user-provided hash function. Duplicates are detected using the compare
 * function and not added. Stores pointer directly without copying.
 * Big O: O(1) average case, O(m) worst case
 */
void addElement(SET *sp, void *elt) {
	assert((elt != NULL) && (sp != NULL));	
	bool found;
	int idx = search(sp, elt, &found);
	if (!found) {
		assert(sp->count < sp->length);
		sp->data[idx] = elt;
		sp->flags[idx] = 'F';
		sp->count++;
	}
}

/* removeElement: remove elt from the hash table if present by marking
 * the slot as Deleted. Does NOT free the element as caller owns the data.
 * Uses the compare function to find the element.
 * Big O: O(1) average case, O(m) worst case
 */
void removeElement(SET *sp, void *elt) {
	assert((sp != NULL) && (elt != NULL));
	bool found;
	int idx = search(sp, elt, &found);
	if (found) {
		sp->flags[idx] = 'D';
		sp->count--;
	}
}

/* findElement: return pointer to stored element equal to elt using hash table
 * lookup with user-provided hash and compare functions, or NULL if not found.
 * Big O: O(1) average case, O(m) worst case
 */
void *findElement(SET *sp, void *elt) {
	assert((sp != NULL) && (elt != NULL));
	bool found;
	int idx = search(sp, elt, &found);
	if (!found) {
		return NULL;
	}
	return sp->data[idx];
}

/* getElements: return a newly-allocated array of generic pointers to the 
 * elements stored in the hash table (only Filled slots).
 * Big O: O(m) - where m is maxElts
 */
void *getElements(SET *sp) {
	assert(sp != NULL);
	void **cpy;
	cpy = malloc(sp->count*sizeof(void *));
	assert(cpy != NULL);
	int j = 0;
	for (int i = 0; i < sp->length; i++) {
		if (sp->flags[i] == 'F') {
			cpy[j] = sp->data[i];
			j++;
		}
	}
	return cpy;
}


/* private helper: returns the index of element in hash table or the index
 * where you would insert it, found using linear probing from the hash value.
 * Uses the user-provided hash function to compute initial index and the
 * compare function to check for equality. Returns the first Deleted slot 
 * encountered if element is not found.
 * Big O: O(1) average case, O(m) worst case
 */
static int search(SET *sp, void *elt, bool *found) {
	assert(sp != NULL);

	int idx = (*sp->hash)(elt) % sp->length;
	int i = 0;
	int firstDeleted = -1;
	int locn;
	
	while (i < sp->length) {
		locn = (idx + i ) % sp->length;
		if (sp->flags[locn] == 'D') {
			if (firstDeleted == -1) {
				firstDeleted = locn;
			}
		}
		else if (sp->flags[locn] == 'E') {
			*found = false;
			if (firstDeleted == -1) {
				return locn;
			}
			return firstDeleted;
		}
		else if (sp->flags[locn] == 'F') {
			if ((*sp->compare)(sp->data[locn], elt) == 0) {
				*found = true;
				return locn;
			}
		}
		i++;
	}
	*found = false; 
	return firstDeleted;
}
