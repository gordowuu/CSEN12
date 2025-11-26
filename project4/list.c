/*
 * Gordon Wu
 * CSEN12
 * 11/04/2025
 *
 * list.c
 * Implementation of a generic doubly-linked list with a circular sentinel node.
 *
 * Big O Time Complexities:
 * createList: O(1)
 * destroyList: O(n) where n is the number of items in the list
 * numItems: O(1)
 * addFirst: O(1)
 * addLast: O(1)
 * removeFirst: O(1)
 * removeLast: O(1)
 * getFirst: O(1)
 * getLast: O(1)
 * removeItem: O(n)
 * findItem: O(n) where n is the number of items in the list
 * getItems: O(n) where n is the number of items in the list
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include <assert.h>

struct list {
    int count;
    struct node *head;
    int (*compare)();
};

struct node {
    void *data;
    struct node *next;
    struct node *prev;
};

typedef struct node NODE;

/* createList: allocate and initialize a new doubly-linked LIST with a circular
 * sentinel head node.
 * Big O: O(1)
 */
LIST *createList(int (*compare)()) {
    LIST *lp;
    lp = malloc(sizeof(LIST));
    assert(lp != NULL);
    lp->count = 0;
    lp->compare = compare;
    lp->head = malloc(sizeof(NODE));
    assert(lp->head != NULL);
    lp->head->next = lp->head;
    lp->head->prev = lp->head;
    return lp;
}

/* destroyList: deallocate all memory associated with the list pointed to by lp.
 * Frees all nodes in the list and the list structure itself.
 * Big O: O(n) where n is the number of items in the list
 */
void destroyList(LIST *lp) {
    assert(lp != NULL);
    NODE *pDel;
    NODE *pPrev = lp->head->prev;
    while (pPrev != lp->head) {
        pDel = pPrev;
        pPrev = pDel->prev;
        free(pDel);
    }
    free(lp->head);
    free(lp);
}

/* numItems: return the number of items in the list pointed to by lp.
 * Big O: O(1)
 */
int numItems(LIST *lp) {
    assert(lp != NULL);
    return lp->count;
}

/* addFirst: add item as the first element in the list pointed to by lp.
 * Big O: O(1)
 */
void addFirst(LIST *lp, void *item) {
    assert(lp != NULL);
    NODE *newNode;
    newNode = malloc(sizeof(NODE));
    assert(newNode != NULL);
    newNode->data = item;
    newNode->prev = lp->head;
    newNode->next = lp->head->next;
    lp->head->next->prev = newNode;
    lp->head->next = newNode;
    lp->count++;
}

/* addLast: add item as the last element in the list pointed to by lp.
 * Big O: O(1)
 */
void addLast(LIST *lp, void *item) {
    assert(lp != NULL);
    NODE *newNode;
    newNode = malloc(sizeof(NODE));
    assert(newNode != NULL);
    newNode->data = item;
    newNode->next = lp->head;
    newNode->prev = lp->head->prev;
    lp->head->prev->next = newNode;
    lp->head->prev = newNode;
    lp->count++;
}

/* removeFirst: remove and return the first item in the list pointed to by lp.
 * Big O: O(1)
 */
void *removeFirst(LIST *lp) {
    assert(lp != NULL && lp->count > 0);
    NODE *pDel = lp->head->next;
    lp->head->next = pDel->next;
    pDel->next->prev = lp->head;
    void *pDelData = pDel->data;
    free(pDel);
    lp->count--;
    return pDelData;
}

/* removeLast: remove and return the last item in the list pointed to by lp.
 * Big O: O(1)
 */
void *removeLast(LIST *lp){
    assert(lp != NULL && lp->count > 0);
    NODE *pDel = lp->head->prev;
    lp->head->prev = pDel->prev;
    pDel->prev->next = lp->head;
    void *pDelData = pDel->data;
    free(pDel);
    lp->count--;
    return pDelData;
}

/* getFirst: return, but do not remove, the first item in the list pointed to by lp.
 * Big O: O(1)
 */
void *getFirst(LIST *lp) {
    assert(lp != NULL && lp->count > 0);
    return lp->head->next->data;
}

/* getLast: return, but do not remove, the last item in the list pointed to by lp.
 * Big O: O(1)
 */
void *getLast(LIST *lp) {
    assert(lp != NULL && lp->count > 0);
    return lp->head->prev->data;
}

/* removeItem: remove the first occurrence of item from the list pointed to by lp.
 * Big O: O(n) where n is the number of items in the list
 */
void removeItem(LIST *lp, void *item) {
    assert(lp != NULL && lp->compare != NULL);
    NODE *pCur = lp->head->next;
    while (pCur != lp->head) {
        if (lp->compare(pCur->data, item) == 0) {
            pCur->prev->next = pCur->next;
            pCur->next->prev = pCur->prev;
            free(pCur); 
            lp->count--;
            return;
        }
        pCur = pCur->next;
    }
}

/* findItem: if item is present in the list pointed to by lp then return the 
 * matching item, otherwise return NULL.
 * Big O: O(n) where n is the number of items in the list
 */
void *findItem(LIST *lp, void *item) {
    assert(lp != NULL && lp->compare != NULL);
    NODE *pCur = lp->head->next;
    while (pCur != lp->head) {
        if (lp->compare(pCur->data, item) == 0) {
            return pCur->data;
        }
        pCur = pCur->next;
    }
    return NULL;
}

/* getItems: allocate and return an array of all items in the list pointed to by lp.
 * Big O: O(n) 
 */
void *getItems(LIST *lp) {
    assert(lp != NULL);
    void **elts = malloc(sizeof(void*) * lp->count);
    assert(elts != NULL);
    NODE *pCur = lp->head->next;
    int i = 0;
    while (pCur != lp->head) {
        elts[i] = pCur->data;
        pCur = pCur->next;
        i++;
    }
    return elts;
}
