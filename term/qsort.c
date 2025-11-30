/*
 * File:	qsort.c
 *
 * Copyright:	2020, Darren C. Atkinson
 *
 * Description:	Reads words from a text file whose name is given as the
 *		first and only command-line argument.  The words are stored
 *		in a list that is then sorted using quicksort, and the words
 *		are then displayed in sorted order.
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "list.h"


# define MAX_WORD_LENGTH 30		/* maximum length of a single word */


/*
 * Function:	partition
 *
 * Description:	Choose the first element in the sublist as the pivot and
 *		partition the sublist around the pivot.  Hoare's partition
 *		scheme is used: https://en.wikipedia.org/wiki/Quicksort.
 */

static int partition(LIST *lp, int lo, int hi)
{
    int i, j;
    char *temp, *x;


    x = getItem(lp, lo);
    i = lo - 1;
    j = hi + 1;

    while (i < j) {
	do
	    j = j - 1;
	while (strcmp(getItem(lp, j), x) > 0);

	do
	    i = i + 1;
	while (strcmp(getItem(lp, i), x) < 0);

	if (i < j) {
	    temp = getItem(lp, i);
	    setItem(lp, i, getItem(lp, j));
	    setItem(lp, j, temp);
	}
    }

    return j;
}


/*
 * Function:	quickSort
 *
 * Description:	Recursively sort the given sublist using the quicksort
 *		sorting algorithm: partition the sublist around a pivot,
 *		recursively sort the lower half of the list, and finally
 *		recursively sort the upper half of the list.
 */

static void quickSort(LIST *lp, int lo, int hi)
{
    int i;


    if (hi > lo) {
	i = partition(lp, lo, hi);
	quickSort(lp, lo, i);
	quickSort(lp, i + 1, hi);
    }
}


/*
 * Function:	main
 *
 * Description:	Driver function for the qsort application.
 */

int main(int argc, char *argv[])
{
    FILE *fp;
    LIST *words;
    char word[MAX_WORD_LENGTH+1];


    /* Check the number of arguments and try to open the file. */

    if (argc != 2) {
	fprintf(stderr, "missing filename\n");
	exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL) {
	fprintf(stderr, "cannot open file\n");
	exit(EXIT_FAILURE);
    }


    /* Read each word into the buffer and add it to the list. */

    words = createList();

    while (fscanf(fp, "%s", word) == 1)
	addLast(words, strdup(word));

    fclose(fp);


    /* Sort the words in the list and print them out in sorted order. */

    quickSort(words, 0, numItems(words) - 1);

    while (numItems(words) > 0)
	printf("%s\n", (char *) removeFirst(words));

    destroyList(words);
    exit(EXIT_SUCCESS);
}
