/*
 * Gordon Wu
 * CSEN12
 * 11/30/2025
 *
 * list.c
 * implementation of a list abstract data type using a doubly-linked list
 * of nodes, where each node contains a circular queue (using an array).
 * O(1) stack/queue operations and O(n/m) indexing where
 * n is total items and m is items per node.
 *
 *
 * Big O Time Complexities:
 * createList: O(1)
 * destroyList: O(n) where n is total number of items
 * numItems: O(1)
 * addFirst: O(1) average
 * addLast: O(1) average
 * removeFirst: O(1) average
 * removeLast: O(1) average
 * getFirst: O(1)
 * getLast: O(1)
 * getItem: O(n/m) where m is average items per node
 * setItem: O(n/m) where m is average items per node
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"

#define INIT_LENGTH 8

struct node {
    void **data;        /* circular queue array */
    int length;         /* capacity of array */
    int count;          /* number of items in array */
    int first;          /* index of first item */
    struct node *next;
    struct node *prev;
};

struct list {
    int count;          /* total number of items in list */
    struct node *head;  /* sentinel node */
};

typedef struct node NODE;

/* createNode: allocate and initialize a new node with the given array length.
 * Big O: O(1)
 */
static NODE *createNode(int length) {
    NODE *np;
    np = malloc(sizeof(NODE));
    assert(np != NULL);
    np->data = malloc(sizeof(void *) * length);
    assert(np->data != NULL);
    np->length = length;
    np->count = 0;
    np->first = 0;
    np->next = NULL;
    np->prev = NULL;
    return np;
}

/* destroyNode: deallocate memory associated with a node.
 * Big O: O(1)
 */
static void destroyNode(NODE *np) {
    assert(np != NULL);
    free(np->data);
    free(np);
}

/* createList: allocate and initialize a new empty list with a sentinel node.
 * Big O: O(1)
 */
LIST *createList(void) {
    LIST *lp;
    lp = malloc(sizeof(LIST));
    assert(lp != NULL);
    lp->count = 0;
    lp->head = createNode(INIT_LENGTH);
    lp->head->next = lp->head;
    lp->head->prev = lp->head;
    return lp;
}

/* destroyList: deallocate all memory associated with the list pointed to by lp.
 * Big O: O(n) where n is total items (traverses all nodes)
 */
void destroyList(LIST *lp) {
    NODE *np, *next;
    assert(lp != NULL);
    np = lp->head->next;
    while (np != lp->head) {
        next = np->next;
        destroyNode(np);
        np = next;
    }
    destroyNode(lp->head);
    free(lp);
}

/* numItems: return the total number of items in the list pointed to by lp.
 * Big O: O(1)
 */
int numItems(LIST *lp) {
    assert(lp != NULL);
    return lp->count;
}

/* addFirst: add item as the first item in the list pointed to by lp.
 * If first node is full or list is empty, create new node.
 * Big O: O(1) amortized
 */
void addFirst(LIST *lp, void *item) {
    NODE *np;
    assert(lp != NULL);

    np = lp->head->next;
    /* If list empty or first node full, create new node */
    if (np == lp->head || np->count == np->length) {
        int newLen = (np == lp->head) ? INIT_LENGTH : np->length * 2;
        NODE *newNode = createNode(newLen);
        newNode->prev = lp->head;
        newNode->next = lp->head->next;
        lp->head->next->prev = newNode;
        lp->head->next = newNode;
        np = newNode;
    }

    /* Add to front of circular queue in first node */
    np->first = (np->first - 1 + np->length) % np->length;
    np->data[np->first] = item;
    np->count++;
    lp->count++;
}

/* addLast: add item as the last item in the list pointed to by lp.
 * If last node is full or list is empty, create new node.
 * Big O: O(1) amortized
 */
void addLast(LIST *lp, void *item) {
    NODE *np;
    int idx;
    assert(lp != NULL);

    np = lp->head->prev;
    /* If list empty or last node full, create new node */
    if (np == lp->head || np->count == np->length) {
        int newLen = (np == lp->head) ? INIT_LENGTH : np->length * 2;
        NODE *newNode = createNode(newLen);
        newNode->next = lp->head;
        newNode->prev = lp->head->prev;
        lp->head->prev->next = newNode;
        lp->head->prev = newNode;
        np = newNode;
    }

    /* Add to back of circular queue in last node */
    idx = (np->first + np->count) % np->length;
    np->data[idx] = item;
    np->count++;
    lp->count++;
}

/* removeFirst: remove and return the first item in the list pointed to by lp.
 * If first node becomes empty, deallocate it.
 * Big O: O(1) amortized
 */
void *removeFirst(LIST *lp) {
    NODE *np;
    void *item;
    assert(lp != NULL && lp->count > 0);

    np = lp->head->next;

    /* If first node is empty, remove it and move to next */
    while (np->count == 0) {
        NODE *pDel = np;
        np = np->next;
        pDel->prev->next = pDel->next;
        pDel->next->prev = pDel->prev;
        destroyNode(pDel);
    }

    /* Remove from front of circular queue */
    item = np->data[np->first];
    np->first = (np->first + 1) % np->length;
    np->count--;
    lp->count--;

    /* If node is now empty, deallocate it */
    if (np->count == 0) {
        np->prev->next = np->next;
        np->next->prev = np->prev;
        destroyNode(np);
    }

    return item;
}

/* removeLast: remove and return the last item in the list pointed to by lp.
 * If last node becomes empty, deallocate it.
 * Big O: O(1) amortized
 */
void *removeLast(LIST *lp) {
    NODE *np;
    void *item;
    int idx;
    assert(lp != NULL && lp->count > 0);

    np = lp->head->prev;

    /* If last node is empty, remove it and move to prev */
    while (np->count == 0) {
        NODE *pDel = np;
        np = np->prev;
        pDel->prev->next = pDel->next;
        pDel->next->prev = pDel->prev;
        destroyNode(pDel);
    }

    /* Remove from back of circular queue */
    idx = (np->first + np->count - 1 + np->length) % np->length;
    item = np->data[idx];
    np->count--;
    lp->count--;

    /* If node is now empty, deallocate it */
    if (np->count == 0) {
        np->prev->next = np->next;
        np->next->prev = np->prev;
        destroyNode(np);
    }

    return item;
}

/* getFirst: return, but do not remove, the first item in the list.
 * Big O: O(1)
 */
void *getFirst(LIST *lp) {
    NODE *np;
    assert(lp != NULL && lp->count > 0);
    np = lp->head->next;
    while (np->count == 0)
        np = np->next;
    return np->data[np->first];
}

/* getLast: return, but do not remove, the last item in the list.
 * Big O: O(1)
 */
void *getLast(LIST *lp) {
    NODE *np;
    int idx;
    assert(lp != NULL && lp->count > 0);
    np = lp->head->prev;
    while (np->count == 0)
        np = np->prev;
    idx = (np->first + np->count - 1 + np->length) % np->length;
    return np->data[idx];
}

/* getItem: return the item at position index in the list pointed to by lp.
 * Searches from the closest end (head or tail) for efficiency.
 * Big O: O(n/m) where m is average items per node
 */
void *getItem(LIST *lp, int index) {
    NODE *np;
    int idx, pos;
    assert(lp != NULL && index >= 0 && index < lp->count);

    /* Search from head (front to back) */
    if (index < lp->count / 2) {
        np = lp->head->next;
        pos = 0;
        while (pos + np->count <= index) {
            pos += np->count;
            np = np->next;
        }
        idx = (np->first + (index - pos)) % np->length;
    } else {
        /* Search from tail (back to front) */
        np = lp->head->prev;
        pos = lp->count;
        while (pos - np->count > index) {
            pos -= np->count;
            np = np->prev;
        }
        idx = (np->first + (index - (pos - np->count))) % np->length;
    }

    return np->data[idx];
}

/* setItem: update the item at position index in the list pointed to by lp.
 * Big O: O(n/m) where m is average items per node
 */
void setItem(LIST *lp, int index, void *item) {
    NODE *np;
    int idx, pos;
    assert(lp != NULL && index >= 0 && index < lp->count);

    /* Search from head (front to back) */
    if (index < lp->count / 2) {
        np = lp->head->next;
        pos = 0;
        while (pos + np->count <= index) {
            pos += np->count;
            np = np->next;
        }
        idx = (np->first + (index - pos)) % np->length;
    } else {
        /* Search from tail (back to front) */
        np = lp->head->prev;
        pos = lp->count;
        while (pos - np->count > index) {
            pos -= np->count;
            np = np->prev;
        }
        idx = (np->first + (index - (pos - np->count))) % np->length;
    }

    np->data[idx] = item;
}
