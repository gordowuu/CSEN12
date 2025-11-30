/*
 * Gordon Wu
 * CSEN12
 * 11/26/2025
 *
 * huffman.c
 * Driver program for Huffman coding file compression. Reads an input file,
 * builds a Huffman tree using a priority queue, displays character frequencies
 * and bit lengths, and calls pack() to compress the file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "pack.h"
#include "pqueue.h"

#define NUM_CHARS 256
#define EOF_MARKER 256

/* mknode: create a new node with the given count.
 */
static struct node *mknode(int count, struct node *left, struct node *right) {
    struct node *np;
    np = malloc(sizeof(struct node));
    assert(np != NULL);
    np->count = count;
    np->parent = NULL;
    if (left != NULL)
        left->parent = np;
    if (right != NULL)
        right->parent = np;
    return np;
}

/* nodecmp: comparison function for the priority queue.
 */
static int nodecmp(struct node *n1, struct node *n2) {
    return (n1->count < n2->count) ? -1 : (n1->count > n2->count);
}

/* depth: calculate the depth of a node in the tree by counting the number
 * of edges from the node to the root.
 */
static int depth(struct node *np) {
    int d = 0;
    assert(np != NULL);
    while (np->parent != NULL) {
        d++;
        np = np->parent;
    }
    return d;
}

/* main: driver function for Huffman compression.
 * argv[1] = input file name, argv[2] = output file name
 */
int main(int argc, char *argv[]) {
    FILE *fp;
    int counts[NUM_CHARS + 1];
    struct node *nodes[NUM_CHARS + 1];
    PQ *pq;
    int i, c, d, bits;
    struct node *left, *right, *parent;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Initialize counts and nodes arrays */
    for (i = 0; i <= NUM_CHARS; i++) {
        counts[i] = 0;
        nodes[i] = NULL;
    }

    /* Count character frequencies from input file */
    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    while ((c = getc(fp)) != EOF) {
        counts[c]++;
    }
    fclose(fp);

    /* Create priority queue and populate with leaf nodes */
    pq = createQueue(nodecmp);
    for (i = 0; i < NUM_CHARS; i++) {
        if (counts[i] > 0) {
            nodes[i] = mknode(counts[i], NULL, NULL);
            addEntry(pq, nodes[i]);
        }
    }

    /* Create leaf node for EOF marker with count 0 */
    nodes[EOF_MARKER] = mknode(0, NULL, NULL);
    addEntry(pq, nodes[EOF_MARKER]);

    /* Build Huffman tree by combining two smallest nodes until one remains */
    while (numEntries(pq) > 1) {
        left = removeEntry(pq);
        right = removeEntry(pq);
        parent = mknode(left->count + right->count, left, right);
        addEntry(pq, parent);
    }

    /* Print character frequencies and bit lengths */
    for (i = 0; i <= NUM_CHARS; i++) {
        if (nodes[i] != NULL) {
            d = depth(nodes[i]);
            bits = counts[i] * d;
            if (isprint(i)) {
                printf("'%c': %d x %d bits = %d bits\n", i, counts[i], d, bits);
            } else {
                printf("%03o: %d x %d bits = %d bits\n", i, counts[i], d, bits);
            }
        }
    }

    /* Call pack to compress the file */
    pack(argv[1], argv[2], nodes);

    /* Clean up */
    destroyQueue(pq);
    return 0;
}
