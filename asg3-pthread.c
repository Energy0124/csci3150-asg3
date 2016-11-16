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
#include <stdint.h>
#include <sys/stat.h>


//#define BUFFER_SIZE 1024 + 1
//#define TRUE_BUFFER_SIZE BUFFER_SIZE+1

//TODO:: Change back to 100000000
#define HASHTABLE_SIZE 100000000+1

#define WRITE_BUFFER_SIZE 65536

#define STR_BUFFER_SIZE 67

/* Function Declaration */
double timedifference_msec(struct timeval t0, struct timeval t1);

extern int *readdata(char *filename, long *number);

void printTimeElapsed(struct timeval *t0, struct timeval *t1, double elapsed);

/* Main */

static char hashtable[HASHTABLE_SIZE] = {0};
static char hashtable2[HASHTABLE_SIZE] = {0};
static int cache[4][HASHTABLE_SIZE / 4 + 1] = {0};

//static int cacheIndex[4]={0};
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
char *itoaBase10(char *buf, uint32_t val) {
    char *p = &buf[10];

    *p = '\0';

    while (val >= 100) {
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
    if (x >= 100000) {
        if (x >= 10000000) {
            if (x >= 1000000000) return 10;
            if (x >= 100000000) return 9;
            return 8;
        }
        if (x >= 1000000) return 7;
        return 6;
    } else {
        if (x >= 1000) {
            if (x >= 10000) return 5;
            return 4;
        } else {
            if (x >= 100) return 3;
            if (x >= 10) return 2;
            return 1;
        }
    }
}

typedef struct readdataParameter {
    char *filename;
    long *number;
    int *array;
} ReadParameter;

typedef struct insertParameter {
    pthread_t readThread_tid;
    int tableID;
} InsertParameter;

typedef struct subInsertParameter {
    int start;
    int end;
    int tableID;
} SubInsertParameter;


int *array1, *array2;
long num1, num2;
int numOfThread = 0;
int maxNumOfThread = 0;
pthread_t readThread_tid1, readThread_tid2;
pthread_t insertThread_tid1, insertThread_tid2;
ReadParameter readParameter1;
ReadParameter readParameter2;
InsertParameter insertParameter1;
InsertParameter insertParameter2;
struct timeval t0;
struct timeval t1;
double elapsed;


void *parallel_readdata(void *parameter) {
    ReadParameter *readParameter = (ReadParameter *) parameter;
    printf("Start reading %s \n", (*readParameter).filename);
    printTimeElapsed(&t0, &t1, elapsed);
    (*readParameter).array = readdata((*readParameter).filename, (*readParameter).number);
    printf("Done reading %s \n", (*readParameter).filename);
    printTimeElapsed(&t0, &t1, elapsed);
}

void *sub_parallel_insert(void *parameter) {

    int start = (*((SubInsertParameter *) parameter)).start;
    int end = (*((SubInsertParameter *) parameter)).end;
    printf("Start sub inserting array %d, from %d to %d \n", (*((SubInsertParameter *) parameter)).tableID,start, end);
    printTimeElapsed(&t0, &t1, elapsed);
    int *array = NULL;
    char *table = NULL;
    if ((*((SubInsertParameter *) parameter)).tableID == 1) {
        array = array1;
        table = hashtable;
    } else if ((*((SubInsertParameter *) parameter)).tableID == 2) {
        array = array2;
        table = hashtable2;
    }
    int i;
    for (i = start; i < end; ++i) {
        table[array[i]] = 1;
    }
    printf("Done sub inserting array %d, from %d to %d \n", (*((SubInsertParameter *) parameter)).tableID,start, end);
    printTimeElapsed(&t0, &t1, elapsed);
}

void *parallel_insert(void *parameter) {


    InsertParameter *insertParameter = (InsertParameter *) parameter;

    pthread_join((*insertParameter).readThread_tid, NULL);
    numOfThread--;

    if ((*insertParameter).tableID == 1) {
        array1 = readParameter1.array;
    } else if ((*insertParameter).tableID == 2) {
        array2 = readParameter2.array;
    }

    printf("Read Thread %d joined\n", (*insertParameter).tableID);
    printTimeElapsed(&t0, &t1, elapsed);

    printf("Start inserting array %d \n", (*insertParameter).tableID);
    printTimeElapsed(&t0, &t1, elapsed);

    pthread_t tid1, tid2;

    if ((*insertParameter).tableID == 1) {
        printf("Start inserting first file data to array.\n");
        printTimeElapsed(&t0, &t1, elapsed);
        if (maxNumOfThread > 2) {

            if (maxNumOfThread == 3) {
                SubInsertParameter subInsertParameter1;
                subInsertParameter1.tableID = 1;
                subInsertParameter1.start = 0;
                subInsertParameter1.end = num1 - 1 - num1 / (maxNumOfThread - 1);
                pthread_create(&tid1, NULL, sub_parallel_insert, &subInsertParameter1);
            } else if (maxNumOfThread > 3) {
                SubInsertParameter subInsertParameter1;
                subInsertParameter1.tableID = 1;
                subInsertParameter1.start = 0;
                subInsertParameter1.end = num1 - 1 - ((num1 / (maxNumOfThread - 1)) * 2);
                pthread_create(&tid1, NULL, sub_parallel_insert, &subInsertParameter1);
                SubInsertParameter subInsertParameter2;
                subInsertParameter2.tableID = 1;
                subInsertParameter2.start = num1 - 1 - ((num1 / (maxNumOfThread - 1)) * 2);
                subInsertParameter2.end = num1 - 1 - num1 / (maxNumOfThread - 1);
                pthread_create(&tid2, NULL, sub_parallel_insert, &subInsertParameter2);
            }

            printf("Start sub inserting array 1, from %d to %d \n", num1 - 1 - num1 / (maxNumOfThread - 1), num1);
            printTimeElapsed(&t0, &t1, elapsed);

            unsigned int i = 0;
            for (i = num1 - 1 - num1 / (maxNumOfThread - 1); i < num1; i++) {

                hashtable[array1[i]] = 1;
            }
            printf("Done sub inserting array 1, from %d to %d \n", num1 - 1 - num1 / (maxNumOfThread - 1), num1);
            printTimeElapsed(&t0, &t1, elapsed);
            if (maxNumOfThread == 3) {
                pthread_join(tid1, NULL);
            } else if (maxNumOfThread > 3) {
                pthread_join(tid1, NULL);
                pthread_join(tid2, NULL);
            }

        } else {
            unsigned int i = 0;
            for (i = 0; i < num1; i++) {

                hashtable[array1[i]] = 1;
            }

        }
        printf("Inserting first file data completed.\n");
        printTimeElapsed(&t0, &t1, elapsed);
    } else if ((*insertParameter).tableID == 2) {
        printf("Start inserting second file data to array.\n");
        printTimeElapsed(&t0, &t1, elapsed);

        if (maxNumOfThread > 2) {
            if (maxNumOfThread == 3) {
                SubInsertParameter subInsertParameter1;
                subInsertParameter1.tableID = 2;
                subInsertParameter1.start = 0;
                subInsertParameter1.end = num2 - 1 - num2 / (maxNumOfThread - 1);
                pthread_create(&tid1, NULL, sub_parallel_insert, &subInsertParameter1);
            } else if (maxNumOfThread > 3) {
                SubInsertParameter subInsertParameter1;
                subInsertParameter1.tableID = 2;
                subInsertParameter1.start = 0;
                subInsertParameter1.end = num2 - 1 - ((num2 / (maxNumOfThread - 1)) * 2);
                pthread_create(&tid1, NULL, sub_parallel_insert, &subInsertParameter1);
                SubInsertParameter subInsertParameter2;
                subInsertParameter2.tableID = 2;
                subInsertParameter2.start = num2 - 1 - ((num2 / (maxNumOfThread - 1)) * 2);
                subInsertParameter2.end = num2 - 1 - num2 / (maxNumOfThread - 1);
                pthread_create(&tid2, NULL, sub_parallel_insert, &subInsertParameter2);
            }

            printf("Start sub inserting array 2, from %d to %d \n", num2 - 1 - num2 / (maxNumOfThread - 1), num2);
            printTimeElapsed(&t0, &t1, elapsed);
            unsigned int i = 0;
            for (i = num2 - 1 - num2 / (maxNumOfThread - 1); i < num2; i++) {

                hashtable2[array2[i]] = 1;
            }
            printf("Done sub inserting array 2, from %d to %d \n", num2 - 1 - num2 / (maxNumOfThread - 1), num2);
            printTimeElapsed(&t0, &t1, elapsed);
            if (maxNumOfThread == 3) {
                pthread_join(tid1, NULL);
            } else if (maxNumOfThread > 3) {
                pthread_join(tid1, NULL);
                pthread_join(tid2, NULL);
            }
        } else {
            unsigned int i = 0;
            for (i = 0; i < num2; i++) {

                hashtable2[array2[i]] = 1;
            }
        }
        /*    for (i = 0; i < num2; i++) {

            if (hashtable[array2[i]] == 1) {
                hashtable[array2[i]] = 3;

            } else if (hashtable[array2[i]] == 0) {
                hashtable[array2[i]] = 2;
            }
        }*/
        printf("Inserting second file data completed.\n");
        printTimeElapsed(&t0, &t1, elapsed);
    }


    printf("Done inserting array %d \n", (*insertParameter).tableID);
    printTimeElapsed(&t0, &t1, elapsed);
}

int main(int argc, char *argv[]) {


    gettimeofday(&t0, 0);

    if (argc != 5) {
        printf("usage:\n");
        printf("    ./asg3-pthread inputfile1 inputfile2 outputfile ThreadNum\n");

        return -1;
    }

    maxNumOfThread = atoi(argv[4]);


    if (maxNumOfThread > 1) {


        printf("Start checking file size\n");
        printTimeElapsed(&t0, &t1, elapsed);

        struct stat st, st2;
        stat(argv[1], &st);
        stat(argv[2], &st2);
        long size = st.st_size;
        long size2 = st2.st_size;
        printf("Done checking file size\n");
        printTimeElapsed(&t0, &t1, elapsed);

        if (size > size2) {
            readParameter1.filename = argv[1];
            readParameter1.number = &num1;
            readParameter1.array = array1;

            readParameter2.filename = argv[2];
            readParameter2.number = &num2;
            readParameter2.array = array2;
        } else {
            readParameter1.filename = argv[2];
            readParameter1.number = &num1;
            readParameter1.array = array1;

            readParameter2.filename = argv[1];
            readParameter2.number = &num2;
            readParameter2.array = array2;

        }

        pthread_create(&readThread_tid1, NULL, parallel_readdata, &readParameter1);
        numOfThread++;
        printf("Read Thread 1 created \n");
        printTimeElapsed(&t0, &t1, elapsed);


        pthread_create(&readThread_tid2, NULL, parallel_readdata, &readParameter2);
        numOfThread++;
        printf("Read Thread 2 created \n");
        printTimeElapsed(&t0, &t1, elapsed);
/*
        pthread_join(readThread_tid1, NULL);
        array1 = readParameter1.array;
        printf("Read Thread 1 joined \n");
        printTimeElapsed(&t0, &t1, elapsed);
        pthread_join(readThread_tid2, NULL);
        array2 = readParameter2.array;
        printf("Read Thread 2 joined \n");
        printTimeElapsed(&t0, &t1, elapsed);
*/


    } else {

        array1 = readdata(argv[1], &num1);
        printf("Read file1.\n");
        printTimeElapsed(&t0, &t1, elapsed);
        array2 = readdata(argv[2], &num2);
        printf("Read file2.\n");
        printTimeElapsed(&t0, &t1, elapsed);
    }




//    array1 = readdata(argv[1], &num1);
//    array2 = readdata(argv[2], &num2);
    /* do your assignment start from here */

    if (maxNumOfThread > 1) {
        insertParameter1.tableID = 1;
        insertParameter1.readThread_tid = readThread_tid1;
        pthread_create(&insertThread_tid1, NULL, parallel_insert, &insertParameter1);
        numOfThread++;

        printf("Insert Thread 1 created \n");
        printTimeElapsed(&t0, &t1, elapsed);

        insertParameter2.tableID = 2;
        insertParameter2.readThread_tid = readThread_tid2;
        pthread_create(&insertThread_tid2, NULL, parallel_insert, &insertParameter2);
        numOfThread++;

        printf("Insert Thread 2 created \n");
        printTimeElapsed(&t0, &t1, elapsed);
    } else {

        printf("Start inserting first file data to array.\n");
        printTimeElapsed(&t0, &t1, elapsed);
        unsigned int i = 0;
        for (i = 0; i < num1; i++) {

            hashtable[array1[i]] = 1;
        }
        printf("Inserting first file data completed.\n");
        printTimeElapsed(&t0, &t1, elapsed);


        printf("Start inserting second file data to array.\n");
        printTimeElapsed(&t0, &t1, elapsed);
        for (i = 0; i < num2; i++) {

            hashtable2[array2[i]] = 1;
        }
        /*    for (i = 0; i < num2; i++) {

            if (hashtable[array2[i]] == 1) {
                hashtable[array2[i]] = 3;

            } else if (hashtable[array2[i]] == 0) {
                hashtable[array2[i]] = 2;
            }
        }*/
        printf("Inserting second file data completed.\n");
        printTimeElapsed(&t0, &t1, elapsed);
    }

    if (maxNumOfThread > 1) {
        pthread_join(insertThread_tid2, NULL);
        numOfThread--;
        printf("Insert Thread 2 joined\n");
        printTimeElapsed(&t0, &t1, elapsed);

        pthread_join(insertThread_tid1, NULL);
        numOfThread--;
        printf("Insert Thread 1 joined\n");
        printTimeElapsed(&t0, &t1, elapsed);
    }


    printf("Start writing to output file.\n");
    printTimeElapsed(&t0, &t1, elapsed);
    char writeBuffer[WRITE_BUFFER_SIZE] = {'\0'};
    FILE *outputFile = fopen(argv[3], "w");
    setvbuf(outputFile, writeBuffer, _IOFBF, WRITE_BUFFER_SIZE);
    char strBuffer[STR_BUFFER_SIZE];
    unsigned int i = 0;
    for (i = 0; i < HASHTABLE_SIZE; i++) {
        if (hashtable[i] && hashtable2[i]) {
            char *str = itoaBase10(strBuffer, i);
            fwrite(str, 1, (size_t) uintlen(i), outputFile);
            fwrite("\n", 1, 1, outputFile);
            //fprintf(outputFile,"%d\n",i);
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
    printf("Code executed in %f milliseconds.\n", elapsed);
}
