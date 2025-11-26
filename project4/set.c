/*
 * Gordon Wu
 * CSEN12
 * 11/09/2025
 *
 * set.c
 * Implementation of a set abstract data type using a hash table with chaining.
 *
 * Big O Time Complexities:
 * createSet: O(m) where m is the number of lists
 * destroySet: O(m) where m is the number of lists
 * numElements: O(1)
 * addElement: O(1) average case, O(n) worst case where n is total elements
 * removeElement: O(1) average case, O(n) worst case where n is total elements
 * findElement: O(1) average case, O(n) worst case where n is total elements
 * getElements: O(n + m) where n is total elements and m is the number of lists
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "set.h"
#include "list.h"

#define ALPHA 20

struct set {
    int length;
    int count;
    int (*compare)();
    unsigned (*hash)();
    LIST **lists;
};


/* createSet: allocate and initialize a new hash table SET with m = maxElts/ALPHA lists.
 * Big O: O(m) where m is the number of lists
 */
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{    
    SET *sp;    
    assert(compare != NULL && hash != NULL);
    assert(maxElts > 0);
    sp = malloc(sizeof(SET));    
    assert(sp != NULL); 
    sp->compare = compare;    
    sp->hash = hash;    
    sp->count = 0;   
    sp->length = maxElts / ALPHA; 
    sp->lists = malloc(sizeof(LIST*) * sp->length);
    assert(sp->lists != NULL);
    for (int i = 0; i < sp->length; i++)
        sp->lists[i] = createList(compare);
    return sp;
}

/* destroySet: deallocate all memory associated with the set pointed to by sp.
 * Destroys all lists in the hash table and frees the set structure itself.
 * Big O: O(m) where m is the number of lists
 */
void destroySet(SET *sp)
{
    assert(sp != NULL);
    for (int i = 0; i < sp->length; i++)
        destroyList(sp->lists[i]);
    free(sp->lists);
    free(sp);
}

/* numElements: return the number of elements in the set pointed to by sp.
 * Big O: O(1)
 */
int numElements(SET *sp)
{
    assert(sp != NULL);
    return sp->count;
}

/* addElement: add elt to the set pointed to by sp if it is not already present.
 * Big O: O(1) average case, O(n) worst case where n is total elements
 */
void addElement(SET *sp, void *elt)
{
    assert(sp != NULL && elt != NULL);
    int index = (*sp->hash)(elt) % sp->length;
    if (findItem(sp->lists[index], elt) == NULL)
    {
        addFirst(sp->lists[index], elt);
        sp->count++;
    }
}

/* removeElement: remove elt from the set pointed to by sp if it exists.
 * Big O: O(1) average case, O(n) worst case where n is total elements
 */
void removeElement(SET *sp, void *elt)
{
    assert(sp != NULL && elt != NULL);
    int index = (*sp->hash)(elt) % sp->length;
    if (findItem(sp->lists[index], elt) != NULL)
    {
        removeItem(sp->lists[index], elt);
        sp->count--;
    }
}

/* findElement: if elt is present in the set pointed to by sp then return the
 * matching element, otherwise return NULL.
 * Big O: O(1) average case, O(n) worst case where n is total elements
 */
void *findElement(SET *sp, void *elt)
{
    assert(sp != NULL && elt != NULL);
    int index = (*sp->hash)(elt) % sp->length;
    return findItem(sp->lists[index], elt); 
}

/* getElements: allocate and return an array of all elements in the set pointed to by sp.
 * Big O: O(n + m) where n is total elements and m is the number of lists
 */
void *getElements(SET *sp)
{
    assert(sp != NULL);
    void **copy = malloc(sizeof(void*) * sp->count);
    assert(copy != NULL);
    int idx = 0;
    for (int i = 0; i < sp->length; i++)
    {
        int num = numItems(sp->lists[i]);
        if (num > 0)
        {
            void **items = getItems(sp->lists[i]);
            memcpy(copy + idx, items, sizeof(void*) * num);
            free(items);
            idx += num;
        }
    }
    return copy; 
}