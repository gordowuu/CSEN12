/*
 * Gordon Wu
 * CSEN12
 * 10/19/2025
 *
 * table.c
 * Implementation of a hash table for strings using open addressing
 * with linear probing. Elements are stored in an array with flags
 * indicating Empty, Filled, or Deleted states.
 *
 * Big O Time Complexities (Average Case):
 * createSet: O(m) - where m is maxElts
 * destroySet: O(m) 
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
	char **data;
    char *flags;
} SET;

unsigned strhash(char *s) {
	unsigned hash = 0;
	assert(s != NULL);
	while ( *s != '\0') {
		hash = 31 * hash + *s++;
	}
	return hash;
}

static int search(SET *sp, char *elt, bool *found);

/* createSet: allocate and initialize a new hash table SET holding up to
 * maxElts elements. Initializes the data array and flags array.
 * Big O: O(m) 
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
	sp->flags = malloc(sizeof(char) * maxElts);
	assert(sp->flags != NULL);
	for (int i = 0; i < maxElts; i++) {
		sp->flags[i] = 'E';
	}
	return sp;
}

/* destroySet: free all strings owned by the set (marked as Filled),
 * the data and flags arrays, and the set structure.
 * Big O: O(m) 
 */
void destroySet(SET *sp) {
	assert(sp != NULL);
	for (int i = 0; i < sp->length; i++) {
		if (sp->flags[i] == 'F') {
			free(sp->data[i]);
		}
	}
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

/* addElement: insert elt in the hash table using linear probing.
 * Duplicates are not added.
 * Big O: O(1) average case, O(m) worst case
 */
void addElement(SET *sp, char *elt) {
	assert((elt != NULL) && (sp != NULL));	
	bool found;
	char* newElt;
	int idx = search(sp, elt, &found);
	if (!found) {
		assert(sp->count < sp->length);
		newElt = strdup(elt);
		assert(newElt != NULL);
		sp->data[idx] = newElt;
		sp->flags[idx] = 'F';
		sp->count++;
	}
}

/* removeElement: remove elt from the hash table if present by marking
 * the slot as Deleted.
 * Big O: O(1) average case, O(m) worst case
 */
void removeElement(SET *sp, char *elt) {
	assert((sp != NULL) && (elt != NULL));
	bool found;
	int idx = search(sp, elt, &found);
	if (found) {
		free(sp->data[idx]);
		sp->flags[idx] = 'D';
		sp->count--;
	}
}

/* findElement: return pointer to stored string equal to elt using hash table
 * lookup, or NULL if not found.
 * Big O: O(1) average case, O(m) worst case
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

/* getElements: return a newly-allocated array of pointers to the elements
 * stored in the hash table (only Filled slots).
 * Big O: O(m) 
 */
char **getElements(SET *sp) {
	assert(sp != NULL);
	char **cpy;
	cpy = malloc(sp->count*sizeof(char *));
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
 * Returns the first Deleted slot encountered if element is not found.
 * Big O: O(1) average case, O(m) worst case
 */
static int search(SET *sp, char *elt, bool *found) {
	assert(sp != NULL);

	int idx = strhash(elt) % sp->length;
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
			if (strcmp(sp->data[locn], elt) == 0) {
				*found = true;
				return locn;
			}
		}
		i++;
	}
	*found = false; 
	return firstDeleted;
}
