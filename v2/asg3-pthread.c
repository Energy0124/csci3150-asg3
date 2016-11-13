/*
* CSCI3150 Assignment 3 - Implement pthread and openmp program
*
*/

/* Header Declaration */
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>


#define BUFFER_SIZE 1024 + 1
//#define TRUE_BUFFER_SIZE BUFFER_SIZE+1

//TODO:: Change back to 1000000000
#define HASHTABLE_SIZE 1000000000

/* Function Declaration */
double timedifference_msec(struct timeval t0, struct timeval t1);
extern int *readdata(char *filename, long *number);
/* Main */

static char hashtable[HASHTABLE_SIZE] = {0};
//static bool hashtable1[100000000] = {false};
//static bool hashtable2[100000000] = {false};
//static bool hashtable3[100000000] = {false};
//static bool hashtable4[100000000] = {false};
//static bool hashtable5[100000000] = {false};
//static bool hashtable6[100000000] = {false};
//static bool hashtable7[100000000] = {false};
//static bool hashtable8[100000000] = {false};
//static bool hashtable9[100000000] = {false};

double timedifference_msec(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}






int main(int argc, char *argv[]) {
    struct timeval t0;
    struct timeval t1;
    double elapsed;

    gettimeofday(&t0, 0);

    if(argc!=5) {
        printf("usage:\n");
        printf("    ./asg3-pthread inputfile1 inputfile2 outputfile ThreadNum\n");

        return -1;
    }

    int *array1, *array2;
    long num1, num2;

    array1 = readdata(argv[1], &num1);
    array2 = readdata(argv[2], &num2);

    /* do your assignment start from here */
	//array1 = readdata(argv[1], &num1);
    unsigned long long i = 0;
	for( i=0;i<num1;i++){

        hashtable[array1[i]] = 1;
	}
    printf("First loop finished.\n");
    for( i=0;i<num2;i++){

        if (hashtable[array2[i]] == 1) {
            hashtable[array2[i]] = 3;

        } else if(hashtable[array2[i]] == 0){
            hashtable[array2[i]] = 2;
        }
    }
    for (i = 0; i < HASHTABLE_SIZE; i++) {
        if (hashtable[i]==3) {
           // printf("%llu\n", i);
        }
    }
    printf("Second loop finished.\n");
    gettimeofday(&t1, 0);
    elapsed = timedifference_msec(t0, t1);
    printf("\nCode executed in %f milliseconds.\n", elapsed);
    /* FILE *fp=fopen(argv[3], "w");
     fclose(fp);*/

    return 0;
}
