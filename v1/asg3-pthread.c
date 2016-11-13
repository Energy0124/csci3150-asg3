/*
* CSCI3150 Assignment 3 - Implement pthread and openmp program
* Feel free to modify the given code.
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

#define BUFFER_SIZE 1024+1
//#define TRUE_BUFFER_SIZE BUFFER_SIZE+1

//TODO:: Change back to 1000000000
#define HASHTABLE_SIZE 1000000000

/* Function Declaration */
double timedifference_msec(struct timeval t0, struct timeval t1);

/* Main */

static char hashtable[HASHTABLE_SIZE] = {0};
//static bool hashtable2[HASHTABLE_SIZE] = {false};
//static bool hashtable1[100000000] = {false};
//static bool hashtable2[100000000] = {false};
//static bool hashtable3[100000000] = {false};
//static bool hashtable4[100000000] = {false};
//static bool hashtable5[100000000] = {false};
//static bool hashtable6[100000000] = {false};
//static bool hashtable7[100000000] = {false};
//static bool hashtable8[100000000] = {false};
//static bool hashtable9[100000000] = {false};



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


    int fd1 = open(argv[1], O_RDONLY);

    unsigned long long elementCount1 = 1;
    unsigned long long elementLeft1 = 1;
    char buffer[BUFFER_SIZE] = {'\0'};
    _ssize_t readSize = 0;
    bool firstRead = true;
    while ((readSize = read(fd1, buffer, BUFFER_SIZE - 1)) > 0 && elementLeft1 > 0) {

        int i = (int) (readSize - 1), j = 0;
        while (buffer[i] != ' ' && buffer[i] != '\n') {
            i--;
            j--;
        }
        lseek(fd1, j, SEEK_CUR);
        readSize += j;
        buffer[readSize] = '\0';
        char *saveptr;
        char *token = buffer;
        unsigned long long element = 0;
        //while ((token = strtok_r(token, " ", &saveptr)) != NULL) {
        if (firstRead)elementLeft1 = strtoull(token, &saveptr, 10);
        //printf("Element left1: %lld\n", elementLeft1);

        while (elementLeft1 > 0&&strlen(token)>1) {
            element = strtoull(token, &saveptr, 10);
           // printf("Element: %llu\n", element);
            if (firstRead) {
/*            int i = 0;
            while ( buffer[i] != '\n') { i++; }
            lseek(fd, i, SEEK_CUR);*/
                // elementCount1 = strtoull(token, NULL, 10);
                elementCount1 = element;
                firstRead = false;
                token = saveptr;

            } else {
                //unsigned long long t=strtoull(token,NULL,10);

                hashtable[element] = 1;
                token = saveptr;
                elementLeft1--;

                /* if (elementLeft1 % 1000000 == 0) {
                     printf("Element left: %lld\n", elementLeft1);
                     //printf("%d\n", (elementLeft1 > 0));
                     //printf("%d\n", readSize);
                     if(!elementLeft1>0) break;
                 };*/
                // token = NULL;
            }
        }

        //printf("%d\n",readSize);
        //   write(STDOUT_FILENO, buffer, readSize);
        //   write(STDOUT_FILENO, "\n", sizeof("\n"));

    }


    close(fd1);
    printf("Element left1: %lld\n", elementLeft1);
    printf("Element count1: %lld\n", elementCount1);


    int fd2 = open(argv[2], O_RDONLY);
    unsigned long long elementCount2 = 1;
    unsigned long long elementLeft2 = 1;
    char buffer2[BUFFER_SIZE] = {'\0'};
    readSize = 0;
    firstRead = true;
    while ((readSize = read(fd2, buffer2, BUFFER_SIZE - 1)) > 0 && elementLeft2 > 0) {

        int i = (int) (readSize - 1), j = 0;
        while (buffer2[i] != ' ' && buffer2[i] != '\n') {
            i--;
            j--;
        }
        lseek(fd2, j, SEEK_CUR);
        readSize += j;
        buffer2[readSize] = '\0';
        char *saveptr;
        char *token = buffer2;
        unsigned long long element = 0;
        //while ((token = strtok_r(token, " ", &saveptr)) != NULL) {
        if (firstRead) elementLeft2 = strtoull(token, &saveptr, 10);
       // printf("Element left2: %lld\n", elementLeft2);

        while (elementLeft2 > 0&&strlen(token)>1) {
            element = strtoull(token, &saveptr, 10);
            //printf("Element: %llu\n", element);
            if (firstRead) {
/*            int i = 0;
            while ( buffer2[i] != '\n') { i++; }
            lseek(fd, i, SEEK_CUR);*/
                // elementCount2 = strtoull(token, NULL, 10);
                elementCount2 = element;
                firstRead = false;
                token = saveptr;

            } else {
                //unsigned long long t=strtoull(token,NULL,10);
                if (hashtable[element] == 1) {
                    hashtable[element] = 3;

                } else if(hashtable[element] == 0){
                    hashtable[element] = 2;
                }
                token = saveptr;
                elementLeft2--;

                /* if (elementLeft2 % 1000000 == 0) {
                     printf("Element left: %lld\n", elementLeft2);
                     //printf("%d\n", (elementLeft2 > 0));
                     //printf("%d\n", readSize);
                     if(!elementLeft2>0) break;
                 };*/
                // token = NULL;
            }
        }

        //printf("%d\n",readSize);
        //   write(STDOUT_FILENO, buffer2, readSize);
        //   write(STDOUT_FILENO, "\n", sizeof("\n"));

    }

    close(fd2);
    printf("Element left2: %lld\n", elementLeft2);
    printf("Element count2: %lld\n", elementCount2);


    unsigned long long i = 0;
    for (i = 0; i < HASHTABLE_SIZE; i++) {
        if (hashtable[i] == 3) {
     //       printf("%llu\n", i);
        }
    }

/*
    FILE *fp=fopen(argv[3],"w");
    fclose(fp);
*/

    gettimeofday(&t1, 0);
    elapsed = timedifference_msec(t0, t1);
    printf("\nCode executed in %f milliseconds.\n", elapsed);

    //exit(0);
    return 0;
}


double timedifference_msec(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_usec - t0.tv_usec) / 1000.0;
}
