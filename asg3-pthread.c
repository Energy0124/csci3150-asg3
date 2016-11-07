/*
* CSCI3150 Assignment 3 - Implement pthread and openmp program
* Feel free to modify the given code.
*
*/

/* Header Declaration */
#include <stdio.h>
#include <pthread.h>

/* Function Declaration */

/* Main */

int main(int argc, char *argv[]) {
    if(argc!=5) {
        printf("usage:\n");
        printf("    ./asg3-pthread inputfile1 inputfile2 outputfile ThreadNum\n");

        return -1;
    }

    FILE *fp=fopen(argv[3],"w");
    fclose(fp);

    return 0;
}
