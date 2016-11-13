#include <stdio.h>
#include <stdlib.h>
int *readdata(char *filename, long *number){
    FILE *file = fopen(filename, "r");
    file = fopen(filename, "r");
    if(!fscanf(file, "%ld", number)) {
        return NULL;
    }
    int *array = (int *)malloc(*number *sizeof(int));
    long i;
    for(i=0; i<*number; i++) {
        if(!fscanf(file, "%d", array+i)) {
            return  NULL;
        }
    }
    fclose(file);
    return array;
}
