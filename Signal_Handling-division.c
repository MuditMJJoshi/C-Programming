////////////////////////////////////////////////////////////////////////////////
// Main File:         intdate.c  sendsig.c  division.c
// This File:         division.c
// Other Files:       None
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
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int count;//the number of completed success

void handler_SIGFPE(int sig) {
	printf("Error: a division by 0 operation was attemped.\n");
	printf("Total number of operations completed successfully: %i\n", count);
	printf("The program will be terminated.\n");
	exit(0);
}

void  handler_SIGINT (int sig) {
	printf("\nTotal number of operations successfully completed: %i\n", count);
	printf("The program will be terminated.\n");
	exit(0);
}

int main() {
	count = 0;

	struct sigaction act;
	memset (&act, 0, sizeof(act));
	act.sa_handler = handler_SIGFPE;
	if (sigaction(SIGFPE, &act, NULL) < 0) {
		perror("Error handling");
		exit(0);
	}

	struct sigaction act2;
	memset (&act2, 0, sizeof(act2));
	act2.sa_handler = handler_SIGINT;	
	if (sigaction (SIGINT, &act2, NULL) < 0) {
		perror ("Error handling");
		exit(0);
	}

	while(1) {
		char buffer [100];
		printf("Enter first integer: ");
		fgets(buffer, 100, stdin);
		int firstNum = atoi(buffer);
		printf("Enter second integer: ");
		fgets(buffer, 100, stdin);
		int secondNum = atoi(buffer);
		int answer = firstNum / secondNum;
		int remainder = firstNum % secondNum;
		printf("%i / %i is %i with a remainder of %i\n", firstNum, secondNum, answer, remainder);
		count++;
	}
	return (0);
}