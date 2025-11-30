/*
 * Gordon Wu
 * CSEN12
 * 11/26/2025
 *
 * pqueue.c
 * Implementation of a priority queue abstract data type using a binary heap
 * with a dynamically resizing array.
 *
 * Big O Time Complexities:
 * createQueue: O(1)
 * destroyQueue: O(1)
 * numEntries: O(1)
 * addEntry: O(log n) - reheap up
 * removeEntry: O(log n) - reheap down
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pqueue.h"

#define INIT_LENGTH 10
#define PARENT(x) (((x) - 1) / 2)
#define LEFT(x) ((x) * 2 + 1)
#define RIGHT(x) ((x) * 2 + 2)

struct pqueue {
    int count;
    int length;
    void **data;
    int (*compare)();
};

/* createQueue: allocate and initialize a new priority queue using the given
 * comparison function.
 * Big O: O(1)
 */
PQ *createQueue(int (*compare)()) {
    PQ *pq;
    assert(compare != NULL);
    pq = malloc(sizeof(PQ));
    assert(pq != NULL);
    pq->count = 0;
    pq->length = INIT_LENGTH;
    pq->compare = compare;
    pq->data = malloc(sizeof(void *) * pq->length);
    assert(pq->data != NULL);
    return pq;
}

/* destroyQueue: deallocate all memory associated with the priority queue
 * pointed to by pq.
 * Big O: O(1)
 */
void destroyQueue(PQ *pq) {
    assert(pq != NULL);
    free(pq->data);
    free(pq);
}

/* numEntries: return the number of entries in the priority queue pointed
 * to by pq.
 * Big O: O(1)
 */
int numEntries(PQ *pq) {
    assert(pq != NULL);
    return pq->count;
}

/* addEntry: add entry to the priority queue pointed to by pq. Uses reheap up
 * to maintain heap property. Doubles array size when capacity is reached.
 * Big O: O(log n)
 */
void addEntry(PQ *pq, void *entry) {
    int idx;
    assert(pq != NULL && entry != NULL);

    if (pq->count == pq->length) {
        pq->length *= 2;
        pq->data = realloc(pq->data, sizeof(void *) * pq->length);
        assert(pq->data != NULL);
    }

    idx = pq->count;
    pq->count++;

    while (idx > 0 && pq->compare(entry, pq->data[PARENT(idx)]) < 0) {
        pq->data[idx] = pq->data[PARENT(idx)];
        idx = PARENT(idx);
    }

    pq->data[idx] = entry;
}

/* removeEntry: remove and return the smallest entry from the priority queue
 * pointed to by pq. Uses reheap down to maintain heap property.
 * Big O: O(log n)
 */
void *removeEntry(PQ *pq) {
    void *root, *last;
    int idx, child;
    assert(pq != NULL && pq->count > 0);

    root = pq->data[0];
    pq->count--;
    last = pq->data[pq->count];
    idx = 0;

    while (LEFT(idx) < pq->count) {
        child = LEFT(idx);

        if (RIGHT(idx) < pq->count && pq->compare(pq->data[RIGHT(idx)], pq->data[LEFT(idx)]) < 0) {
            child = RIGHT(idx);
        }

        if (pq->compare(pq->data[child], last) < 0) {
            pq->data[idx] = pq->data[child];
            idx = child;
        } else {
            break;
        }
    }

    if (pq->count > 0) {
        pq->data[idx] = last;
    }

    return root;
}
