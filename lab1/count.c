/*
Gordon Wu
CSEN 12
9/24/2025
count.c: Counts the number of words in a text file
*/

//pre-proccessing
#include <stdio.h>
#include <stdlib.h>
# define MAX_WORD_LENGTH 30

//main function
int main(int argc, char* argv[ ]) {

    //check if file name is missing
    if (argc == 1) {
        printf("file name missing\n");
        return 0;
    }
    
    FILE* fileName = fopen(argv[1], "r");
    int count = 0;
    char* word = malloc(sizeof(char)*MAX_WORD_LENGTH);
    
    //check if file failed to open
    if (fileName == NULL) {
        printf("file failed to open\n");
        return 0;
    }
    

    //while loop to count the number of words (sequence of non white space characters) in the text file
    while (fscanf(fileName, "%s", word) == 1) {
        count++;
    }
    
    printf("%d total words\n", count);
    fclose(fileName);
    return 0;  
}
