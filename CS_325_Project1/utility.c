#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utility.h"



int *randomArray(int size) {
    srand(time(NULL));      // seed number generator

    int* arr = malloc(size * sizeof(int));  // allocate memory

    int i;      // loop counter

    for (i = 0; i < size; i++) {
        arr[i] = rand() % 201 - 100;      // random number in [-100, 100]
    }

    return arr;
}
