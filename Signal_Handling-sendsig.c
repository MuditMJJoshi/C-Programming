////////////////////////////////////////////////////////////////////////////////
// Main File:         intdate.c  sendsig.c  division.c
// This File:         sendsig.c
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



#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: <signal type> <pid>\n");
		exit(0);
	}
	pid_t pid = atoi(argv[2]);
	int result = strcmp(argv[1],"-i");
	if (result > 0) {
		if (kill(pid, SIGUSR1) == -1) {
			perror("Error handlering");
			exit(0);
		}
	} else if (result == 0) { 
		if (kill(pid, SIGINT) ==-1) {
			perror("Error handlering");
			exit(0);
		}
	} else {
		perror("Invalid result");
		exit(0);
	}
	return 0;
}