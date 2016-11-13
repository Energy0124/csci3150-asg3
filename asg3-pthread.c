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


//#define BUFFER_SIZE 1024 + 1
//#define TRUE_BUFFER_SIZE BUFFER_SIZE+1

//TODO:: Change back to 1000000000
#define HASHTABLE_SIZE 1000000000

#define WRITE_BUFFER_SIZE 65536

#define STR_BUFFER_SIZE 67

/* Function Declaration */
double timedifference_msec(struct timeval t0, struct timeval t1);

extern int *readdata(char *filename, long *number);

void printTimeElapsed(struct timeval *t0, struct timeval *t1, double elapsed);

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
static uint16_t const str100p[100] = { /* 0x3030, 0x3130, 0x3230 .. 0x3930, 0x3031 .. 0x3939 */

        0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830, 0x3930,
        0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731, 0x3831, 0x3931,
        0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632, 0x3732, 0x3832, 0x3932,
        0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533, 0x3633, 0x3733, 0x3833, 0x3933,
        0x3034, 0x3134, 0x3234, 0x3334, 0x3434, 0x3534, 0x3634, 0x3734, 0x3834, 0x3934,
        0x3035, 0x3135, 0x3235, 0x3335, 0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935,
        0x3036, 0x3136, 0x3236, 0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936,
        0x3037, 0x3137, 0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737, 0x3837, 0x3937,
        0x3038, 0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938,
        0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839, 0x3939
};

//int to char* in base10
char *itoaBase10(char *buf, uint32_t val)
{
    char *p = &buf[10];

    *p = '\0';

    while(val >= 100)
    {
        uint32_t const old = val;

        p -= 2;
        val /= 100;
        memcpy(p, &str100p[old - (val * 100)], sizeof(uint16_t));
    }

    p -= 2;
    memcpy(p, &str100p[val], sizeof(uint16_t));

    return &p[val < 10];
}

//find len of uint
int uintlen(unsigned x) {
    if(x>=100000) {
        if(x>=10000000) {
            if(x>=1000000000) return 10;
            if(x>=100000000) return 9;
            return 8;
        }
        if(x>=1000000) return 7;
        return 6;
    } else {
        if(x>=1000) {
            if(x>=10000) return 5;
            return 4;
        } else {
            if(x>=100) return 3;
            if(x>=10) return 2;
            return 1;
        }
    }
}


int main(int argc, char *argv[]) {
    struct timeval t0;
    struct timeval t1;
    double elapsed;

    gettimeofday(&t0, 0);

    if (argc != 5) {
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
    unsigned int i = 0;
    for (i = 0; i < num1; i++) {

        hashtable[array1[i]] = 1;
    }
    printf("Inserting first file data to hash table completed.\n");
    printTimeElapsed(&t0, &t1, elapsed);

    for (i = 0; i < num2; i++) {

        if (hashtable[array2[i]] == 1) {
            hashtable[array2[i]] = 3;

        } else if (hashtable[array2[i]] == 0) {
            hashtable[array2[i]] = 2;
        }
    }
    printf("Inserting second file data to hash table completed.\n");
    printTimeElapsed(&t0, &t1, elapsed);

    char writeBuffer[WRITE_BUFFER_SIZE]={'\0'};
    FILE *outputFile = fopen(argv[3], "w");
    setvbuf(outputFile, writeBuffer, _IOFBF, WRITE_BUFFER_SIZE);
    char strBuffer[STR_BUFFER_SIZE];
    for (i = 0; i < HASHTABLE_SIZE; i++) {
        if (hashtable[i] == 3) {
//            char *str=itoaBase10(strBuffer,  i);
//            fwrite(str, 1, (size_t) uintlen(i), outputFile);
//            fwrite("\n", 1, 1, outputFile);
            fprintf(outputFile,"%d\n",i);
        }
    }
    fflush(outputFile);
    fclose(outputFile);

    printf("Write to output file completed.\n");


    printTimeElapsed(&t0, &t1, elapsed);
    /* FILE *fp=fopen(argv[3], "w");
     fclose(fp);*/

    return 0;
}

void printTimeElapsed(struct timeval *t0, struct timeval *t1, double elapsed) {
    gettimeofday(t1, 0);
    elapsed = timedifference_msec((*t0), (*t1));
    printf("\nCode executed in %f milliseconds.\n", elapsed);
}
