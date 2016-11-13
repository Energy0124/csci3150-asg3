/*
* CSCI3150 Assignment 3 - Implement pthread and openmp program
*
*/

/* Header Declaration */
#include <stdio.h>
#include <omp.h>

/* Function Declaration */
extern int *readdata(char *filename, long *number);
/* Main */

int main(int argc, char *argv[]) {
    if(argc!=5) {
        printf("usage:\n");
        printf("    ./asg3-openmp inputfile1 inputfile2 outputfile ThreadNum\n");

        return -1;
    }

    int *array1, *array2;
    long num1, num2;

    array1 = readdata(argv[1], &num1);
    array2 = readdata(argv[2], &num2);

    /* do your assignment start from here */


    FILE *fp=fopen(argv[3], "w");
    fclose(fp);

    return 0;
}
