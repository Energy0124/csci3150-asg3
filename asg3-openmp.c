/*
* CSCI3150 Assignment 3 - Implement pthread and openmp program
* Feel free to modify the given code.
*
*/

/* Header Declaration */
#include <stdio.h>
#include <omp.h>

/* Function Declaration */

/* Main */

int main(int argc, char *argv[]) {
    if(argc!=5) {
        printf("usage:\n");
        printf("    ./asg3-openmp inputfile1 inputfile2 outputfile ThreadNum\n");

        return -1;
    }

    FILE *fp=fopen(argv[3], "w");
    fclose(fp);

    return 0;
}
