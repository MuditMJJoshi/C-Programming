////////////////////////////////////////////////////////////////////////////////
// Main File:         cache2Dcols.c
// This File:         cache2Dcols.c
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


int array[3000][500];

int main() {
  int i;
  int j;
  for (i=0 ; i < 500; i++) {
    for (j=0 ; j < 3000 ; j++) {
      array[j][i] = i + j;
    }
  }
}