#include "sort_bus_lines.h"


#define BUFFER_SIZE 60
#define NO_ERROR 5
#define INVALID_NUM_OF_LINES 0
#define INVALID_LINE_NAME 1
#define INVALID_DISTANCE 2
#define INVALID_DURATION 3
#define ARG_LENGTH 21


/**
 * swaps two BusLine objects.
 * @param i Pointer to the first BusLine object.
 * @param j Pointer to the second BusLine object.
 */

void swap(BusLine * i, BusLine * j){
    BusLine temp = *j;
    *j = *i;
    *i = temp;
}

void quick_sort(BusLine *start, BusLine * end, SortType sort_type){
    if(start < end){
        BusLine *pivot = partition(start, end, sort_type);

        quick_sort(start, pivot, sort_type);
        quick_sort(pivot+1, end, sort_type);
    }
}

/**
 * This function partitions an array of BusLine objects based
 * on the distance field.
 * @param start Pointer to the first element of the array.
 * @param end Pointer to the last element of the array.
 * @return Pointer to the partition point.
 */

BusLine *partition_distance (BusLine *start, BusLine *end){
    BusLine * pivot = end - 1;
    BusLine * i = start - 1;

    for (BusLine * j = start; j<pivot; j++){
        if(j -> distance<=pivot -> distance){
            i ++;
            swap(i, j);
        }
        }
    swap((i + 1),pivot);
    return i + 1;
}

/**
 * This function partitions an array of BusLine objects based
 * on the duration field.
 * @param start Pointer to the first element of the array.
 * @param end Pointer to the last element of the array.
 * @return Pointer to the partition point.
 */
BusLine *partition_duration (BusLine *start, BusLine *end){
    BusLine * pivot = end - 1;
    BusLine * i = start - 1;

    for (BusLine * j = start; j<pivot; j++){
        if(j -> duration<=pivot -> duration){
            i ++;
            swap(i, j);
        }
    }
    swap((i + 1),pivot);
    return i + 1;
}


BusLine *partition (BusLine *start, BusLine *end, SortType sort_type){
    if (sort_type == DISTANCE){
        return partition_distance(start, end);
    }
    else{
        return partition_duration(start,end);
    }
}

void bubble_sort (BusLine *start, BusLine *end) {
    for (int i = 0; i < (end-start)-1; i++) {
        for (BusLine *j = start; j < end - i - 1; j++) {
            BusLine *a_line = j;
            BusLine *b_line = (j+1);
            int name_cmp = strcmp(a_line -> name, b_line -> name);
            if (name_cmp>0) {
                swap(a_line, b_line);
            }
        }
    }
}




