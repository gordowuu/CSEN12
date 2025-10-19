
/*
 * Gordon Wu
 * CSEN12
 * 9/30/2025
 *
 * sorted.c
 * Implementation of an ordered set of strings.
 *
 * Big O Time Complexities:
 * createSet: O(1)
 * destroySet: O(n)
 * numElements: O(1)
 * addElement: O(n) 
 * removeElement: O(n)
 * findElement: O(log n)
 * getElements: O(n)
 * search: O(log n)
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
	char **data;
} SET;

static int search(SET *sp, char *elt, bool *found);

/* createSet: allocate and initialize a new SET holding up to
 * maxElts elements.
 * Big O: O(1)
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

/* addElement: insert elt in the set and keep the set sorted.
 * Big O: O(n)
 */
void addElement(SET *sp, char *elt) {
	assert((elt != NULL) && (sp != NULL) && (sp->count < sp->length));	
	bool found;
	int idx = search(sp, elt, &found);
	if (!found) {
		char* newElt = strdup(elt);
		assert(newElt != NULL);
		for (int i = sp->count; i > idx; i--) {
			sp->data[i] = sp->data[i-1];
		}
		sp->data[idx] = newElt;
		sp->count++;
	}
}

/* removeElement: remove elt if present and keep the set sorted.
 * Big O: O(n)
 */
void removeElement(SET *sp, char *elt) {
	assert((sp != NULL) && (elt != NULL));
	bool found;
	int idx = search(sp, elt, &found);
	if (found) {
		free(sp->data[idx]);
		for (int i = idx+1; i < sp->count; i++) {
			sp->data[i-1] = sp->data[i];
		}
		sp->count--;
	}
}

/* findElement: return pointer to stored string equal to elt, or NULL if not
 * found.
 * Big O: O(log n)
 */
char *findElement(SET *sp, char *elt) {
	assert((sp != NULL) && (elt != NULL));
	bool found;
	int idx = search(sp, elt, &found);
	if (!found) {
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


/* private helper: returns the index of element in set or the index where you would insert it, found using binary search algorithm 
 * Big O: O(log n)
 */
static int search(SET *sp, char *elt, bool *found) {
	assert(sp != NULL);
	int lo, hi, mid; lo = 0; hi = sp->count-1;
	
	while (lo <= hi) {
		mid = (lo + hi)/2;	
		int strDiff = strcmp(elt, sp->data[mid]);
		if (strDiff > 0) {
			lo = mid + 1;
		}
		else if (strDiff < 0) {
			hi = mid -1;
		}
		else {
			*found = true; 
			return mid;
		}
	}
	*found = false; 
	return lo;
}
