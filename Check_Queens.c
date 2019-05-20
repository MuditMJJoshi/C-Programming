////////////////////////////////////////////////////////////////////////////////
// Main File:         Check_Queens.c
// This File:         Check_Queens.c
// Other Files:       check1.txt, check2.txt, check3.txt, check4.txt
// Semester:          CS 354 SPRING 2019
//           
// Author:           Mudit Joshi
// Email:            mjoshi6@wisc.edu
// CS Login:         mudit@cs.wisc.edu
//           
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//           
// Persons:          None     
//                   Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//           
// Online sources:   None 
//                   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h> 
#include <stdlib.h>  
#include <string.h>   

 
char *COMMA = ",";
  
/* 
 * Retrieves from the first line of the input file,
 * the number of rows and columns for the board.
 * 
 * fp: file pointer for input file
 * rows: pointer to number of rows
 * cols: pointer to number of columns
 */
void get_dimensions(FILE *fp, int *rows, int *cols) {  
        char *line = NULL;  
        size_t len = 0;  
        if (getline(&line, &len, fp) == -1) {  
                printf("Error in reading the file\n");  
                exit(1);  
        }  
           
        char *token = NULL; 
        token = strtok(line, COMMA);
        *rows = atoi(token); 
           
        token = strtok(NULL, COMMA); 
        *cols = atoi(token);
}      

/* 
 * Returns mun1 if num1 > num2 
 * Otherwise returns num2.
 * For checking leading and non-leding diagonal
 * 
 * num1: Compare Value1 from check_queens 
 * num2: Compare Value2 from check_queens 
 */
int diagonalValue(int num1, int num2){
    if( num1 >= num2){
        return num1;
    }
    else {
        return num2;
    }
}

/* 
 * Returns 1 if and only if there exists at least one pair
 * of queens that can attack each other.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * rows: number of rows
 * cols: number of columns 
 */
int check_queens(int **board, int rows, int cols) { 

    int Row = 0;
    int Col = 0;
    
    //check column for every queen, Row increase by 1.
    for(int j = 0;j < cols;j++){
        for (int i = 0;i < rows;i++)
            if( (int)((board+i)+j) == 1)
                Row++;
        if(Row >= 2)
            return 1;
        else{
            //reset Row when a single row finished.
            Row =0;
            //return 0;
        }
        //return 0;
    }
    
    //check row for every queen, Col increase by 1.
    for(int i = 0;i<rows;i++){
        for (int j = 0;j<cols;j++)
            if(((int)(board+i)+j) == 1)
                Col++;
        if(Col >= 2)
            return 1;
         else{
            //reset Col when a single row finished.
            Col = 0;
           // return 0;
        }
        //return 0;
    } 
    return 0;
}
 
 


/* 
 * This program prints true if the input file has any pair
 * of queens that can attack each other, and false otherwise
 * 
 * argc: CLA count
 * argv: CLA value 
 */
int main(int argc, char *argv[]) {        

        //TODO: Check if number of command-line arguments is correct.
       if(argc != 2){
        printf("Error In The Input Values");
        exit(1);
      }

        //Open the file and check if it opened successfully.
        FILE *fp = fopen(*(argv + 1), "r");
        if (fp == NULL) {
                printf("Cannot open file for reading\n");
                exit(1);
        }


        //Declare local variables.
        int rows, cols;


        //TODO: Call get_dimensions to retrieve the board dimensions.
          //rows= 
          get_dimensions(fp, &rows, &cols);
       //   cols= get_dimensions(FILE *fp, int *rows, int *cols);

        //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
        //int **array[row][column];
        int **board = malloc(rows*sizeof(int*));
        for (int i =0; i < rows ; i++){
        *(board + i) = malloc(cols*sizeof(int));
        }
        //Read the file line by line.
        //Tokenize each line wrt comma to store the values in your 2D array.
        char *line = NULL;
        size_t len = 0;
        char *token = NULL;
        for (int i = 0; i < rows; i++) {

                if (getline(&line, &len, fp) == -1) {
                        printf("Error while reading the file\n");
                        exit(1);        
                }

                token = strtok(line, COMMA);
                for (int j = 0; j < cols; j++) {
                        //TODO: Complete the line of code below
                        //to initialize your 2D array.
                        /* ADD ARRAY ACCESS CODE HERE */ 
                    // **array[i][j]
                     *(*(board+i)+j)  = atoi(token);
                        token = strtok(NULL, COMMA);    
                }
        }

        //Call the function check_queens and print the appropriate
        //output depending on the function's return value.
        if(((int)(check_queens(board, rows,cols)))== 1){
        
        printf("True");
        
        }
        else{
        
        printf("False");
        
        }
        
        //Free all dynamically allocated memory.
         for(int i = 0; i < rows;i++){
            free(board[i]); 
        }
        //free(board);
        free(board);
        
        //Close the file.
        if (fclose(fp) != 0) {
                printf("Error while closing the file\n");
                exit(1);        
        }

        return 0;
}      
