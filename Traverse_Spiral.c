////////////////////////////////////////////////////////////////////////////////
// Main File:         Traverse.c
// This File:         Traverse.c
// Other Files:       Traverse1.txt, Traverse2.txt
// Semester:          CS 354 Spring 2019
//
// Author:           Mudit Joshi
// Email:            mjoshi6@wisc.edu
// CS Login:         mudit
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          NONE
//                   Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   NONE
//                   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *COMMA = ",";

// Structure representing Matrix
// size: Dimension of the square (size*size)
// array: 2D array of integers
typedef struct _Square {
    int size;
    int **array;
} Square;

/* 
 * Retrieves from the first line of the input file,
 * the size of the square matrix.
 *
 * fp: file pointer for input file
 * size: pointer to size of the square matrix
 */
void get_dimensions(FILE *fp, int *size) {
    char *line = NULL;
    size_t len = 0;
    
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file \n");
        exit(1);
    }
    
    char *token = NULL;
    token = strtok(line, COMMA);
    *size = atoi(token);
}

/* 
 * Traverses a given layer from the square matrix
 *
 * array: Heap allocated 2D square matrix
 * size: size of the 2D square matrix
 * layer: layer number to be traversed
 * op: pointer to the output file
 */
void traverse_layer(int **array, int size, int layer, FILE *outputfile) {

    int row, col;
    //int i,j;
   // row_num = layer;
    //col_num = size-layer-1;
    //corner case: size is odd & the layer is last so only one entry to print
    if(size % 2 == 1 && layer == (size + 1) / 2 - 1){
        //printf("flaginlayer corner \n");
        fprintf(outputfile, "%d\n", *(*(array + layer) + layer));
        return;
    }
    
            row= layer;
            col= size-layer-1;
            //Traverse upper row from left to right with appropriate bounds
            for (int i = layer; i <= col; i++) {
           // printf("one\n");
                fprintf(outputfile, "%d ", *(*(array + row) + (i)));
            }
            
            //Traverse right column from top to bottom with appropriate bounds
             row= size-layer-1;
            col= size-layer-1;
            for (int i=layer+1; i<=row ; i++){
            //printf("two\n");
                fprintf(outputfile, "%d ", *(*(array + i) + (col)));
            }
            
            //Traverse lower row from right to left with appropriate bounds
            row= size-layer-1;
            col= size-layer-2;
            for (int j=col; j>=layer; j--) {
            //printf("three\n");
                fprintf(outputfile, "%d ", *(*(array + row) + (j)));
            }
            
            //Traverse left column from bottom to top with appropriate bounds
            row= size-layer-2;
            col= layer;
            //printf("rownum: %d\n", row_num);
            //printf("col_num: %d\n", col_num);
            //sprintf("layer: %d\n", layer);
            for (int i = row; i >= layer+1; i--) {
            //printf("four\n");
                fprintf(outputfile, "%d ", *(*(array + i) + (col)));
            }
}


/* 
 * Traverses the square matrix spirally
 *
 * square: pointer to a structure that contains 2D square matrix
 * op: pointer to the output file
 */
void traverse_spirally(Square *square, FILE *outputfile) {
    int size = square->size;
    int num_layers = 0;
    num_layers = size/2;
    if(size%2 == 1) {
        num_layers++;
    }
    
    int i;
    
    for(i = 0; i < num_layers; i++) {
        traverse_layer(square->array, size, i, outputfile);
    }
}

/* 
 * This program reads a square matrix from the input file
 * and outputs its spiral order traversal to the output file
 *
 * argc: CLA count
 * argv: CLA value
 */

int main(int argc, char *argv[]) {
    
    //Open the file and check if it opened successfully
    //arc!=3 exit
    if(argc != 3){
        printf("Error In Input Values \n");
        exit(1);
      }
    
    FILE *inputfile = fopen(*(argv + 1), "r");
    if (inputfile == NULL) {
        printf("Cannot open for reading \n");
        exit(1);
    }
    
    
    //Create a structure and initialize its size and array fields appropriately
    Square *pointer = malloc(sizeof(Square));
     pointer->size=0;
   // printf("size: %d\n", ptr->size);
    //Call the function get_dimensions to retrieve size of the square matrix
    

    get_dimensions(inputfile, &(pointer->size));
    
    //Dynamically allocate a 2D array as per the retrieved dimensions

      pointer->array = malloc(sizeof(int*) * (pointer->size));
    if (pointer->array == NULL) {
        printf("Errer malloc can't be null \n");
        exit(1);
    }
    
    for(int i=0; i< pointer->size; i++){
        pointer->array[i]= malloc(sizeof(int)*( pointer->size));
        if (pointer->array == NULL) {
            printf("Errer malloc can't be null \n");
            exit(1);
        }
    }
    
    //Read the file line by line by using the function getline as used in get_dimensions
    //Tokenize each line wrt comma to store the values in the square matrix
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i <pointer->size; i++) {
        
        if (getline(&line, &len, inputfile) == -1) {
            printf("Cannot read the file \n");
            exit(1);
        }
        
        token = strtok(line, COMMA);
        for (int j = 0; j < pointer->size; j++) {
            *(*(pointer->array+i)+j)= atoi(token);
            token = strtok(NULL, COMMA);
        }
    }
    
    //Create a structure and initialize its size and array fields appropriately
  //Square *ptr = malloc(sizeof(Square));
    
    
    //Open the output file
    FILE *outputfile = fopen(*(argv + 2), "w");
    if (outputfile == NULL) {
        printf("Cannot open for writing \n");
        exit(1);
    }
    
    //Call the function traverse_spirally
    // printf("flag2 \n");
    //printf("ptr %d\n",ptr->size);
    traverse_spirally(pointer, outputfile);
    
    //Free the dynamically allocated memory
    for(int i=0; i<pointer->size; i++){
        free(pointer->array[i]);
        pointer->array[i] = NULL;
    }
    free (pointer->array);
    pointer->array = NULL;
    
    //printf("flag3 \n");
    
    //Close the input file
    if (fclose(inputfile) != 0) {
        printf("Error while closing the file \n");
        exit(1);
    }
    
    //Close the output file
    if (fclose(outputfile) != 0) {
        printf("Error while closing the file \n");
        exit(1);
    }
    
    return 0;
}

