#include "test_bus_lines.h"
int is_sorted_by_distance (BusLine *start, BusLine *end){
    int num_of_lines = (end-start);
    for (int i = 0; i < num_of_lines-1; i++){
        BusLine a_line = *(start+i), b_line = *(start+i+1);
        if (a_line.distance > b_line.distance){
            return 0;
        }
    }
    return 1;
}

int is_sorted_by_duration (BusLine *start, BusLine *end){
    int num_of_lines = (end-start);
    for (int i = 1; i < num_of_lines; i++){
        BusLine a_line = *(start+i-1), b_line = *(start+i);
        if (a_line.duration > b_line.duration){
            return 0;
        }
    }
    return 1;
}


int is_sorted_by_name (BusLine *start, BusLine *end){
    int num_of_lines = (end-start);
    for (int i = 1; i < num_of_lines; i++){
        BusLine a_line = *(start+i-1), b_line = *(start+i);
        int name_cmp = strcmp(a_line.name , b_line.name);
        if (name_cmp>0){
            return 0;
        }
    }
    return 1;
}

int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original){
    
    size_t count = 0;
    size_t size_of_sorted = (end_sorted - start_sorted);
    size_t size_of_original =
            (end_original - start_original);
    for(size_t i = 0; i < size_of_sorted; i++){
        for (size_t j = 0; j < size_of_original; j++){
            if (strcmp(start_sorted[i].name, start_original[j].name) ==0){
                count++;
                break;
            }
        }
    }
    if((count == size_of_sorted) &&
            (size_of_sorted ==
       size_of_original)){
        return 1;
    }
    return 0;
}
