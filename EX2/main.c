#include <stdio.h>
#include <stdlib.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"



#define BUFFER_SIZE 60
#define NO_ERROR 5
#define INVALID_LINE_NAME 0
#define INVALID_DISTANCE 1
#define INVALID_DURATION 2
#define INVALID_NUM_OF_LINES 3
#define ARG_LENGTH 21


#define DIGIT_LOWER_BOUND '0'

#define DIGIT_UPPER_BOUND '9'

#define LOWERCASE_LOWER_BOUND 'a'

#define LOWERCASE_UPPER_BOUND 'z'

#define PASS 1

#define MAXIMUM_DISTANCE 1000

#define MINIMUM_DISTANCE 0

#define MINIMUM_DURATION 10

#define MAXIMUM_DURATION 100

#define TEN 10

#define TEN_BASE TEN

enum SortType get_sort_type(char *const *argv);

void create_line_array(enum SortType mode, int error,
        char *buffer, int number_of_lines,
                           BusLine *lines_arr,int i);

void test_routine(int number_of_lines, BusLine *lines_arr);


/** this function detects the argument and set the program to the right
 * mode
 * @param argv - the arguments recieved by main
 * @return
 */
enum SortType get_sort_type(char *const *argv) {
    enum SortType mode;
    if (strcmp(argv[1],"by_duration") == 0){
        mode = DURATION;
    }
    else if(strcmp(argv[1],"by_distance")==0){
        mode = DISTANCE;
    }
    else if(strcmp(argv[1],"by_name")==0){
        mode = NAME;
    }
    else if(strcmp(argv[1],"test")==0){
        mode = TEST;
    }
    else{
        mode =0;
        EXIT_FAILURE;
    }
    return mode;
}

/**
 * this function first call the function create_line_array, and than by
 * considering the mode value call the right kind of sort
 * @param mode an enum for the requested sort (or test)
 * @param error an enum fot the current error value (if any error was detected)
 * @param buffer a buffer to recieve the standart input
 * @param number_of_lines the number of bus lines requested by the user
 * @param lines_arr the lines of the users input
 */
void sort_lines(enum SortType mode, int error,
        char * buffer, int number_of_lines, BusLine * lines_arr){
        create_line_array(mode, error, buffer,
                                             number_of_lines, lines_arr, 0);
        if (mode == DISTANCE || mode == DURATION){
            quick_sort(lines_arr, lines_arr + number_of_lines,
                       mode );
        }
        else if (mode == NAME){
            bubble_sort(lines_arr, lines_arr + number_of_lines);
        }
        else if (mode == TEST){
            test_routine(number_of_lines, lines_arr);
        }
    }

    /**
     * this function calls to the 6 tests requested in the exercise paper
     * @param number_of_lines the number of bus lines requested by the user
     * @param lines_arr an array of buslines requested by the user
     */
void test_routine(int number_of_lines, BusLine *lines_arr) {
    BusLine * original_array= malloc(number_of_lines * sizeof(BusLine));
    if (original_array == NULL){free(original_array);
        free(lines_arr);EXIT_FAILURE;}
    memcpy(original_array, lines_arr,
           number_of_lines* sizeof(BusLine));
    quick_sort(lines_arr, lines_arr + number_of_lines,
               DISTANCE);
    if (is_sorted_by_distance(lines_arr,
                              lines_arr + number_of_lines)==PASS){
        printf("TEST 1 PASSED\n");
    } else{printf("TEST 1 FAILED\n");}
    if (is_equal(lines_arr, lines_arr+number_of_lines,
                 original_array,
                 original_array+number_of_lines) == PASS){
        printf("TEST 2 PASSED\n");}
    else{printf("TEST 2 FAILED\n");}
    quick_sort(lines_arr, lines_arr + number_of_lines,
               DURATION);
    if (is_sorted_by_duration(lines_arr,
                              lines_arr + number_of_lines)==PASS){
        printf("TEST 3 PASSED\n");
    } else{printf("TEST 3 FAILED\n");}
    if (is_equal(lines_arr, lines_arr+number_of_lines,
                 original_array,
                 original_array+number_of_lines) == PASS){
        printf("TEST 4 PASSED\n");}
    else{printf("TEST 4 FAILED\n");}
    bubble_sort(lines_arr,lines_arr+number_of_lines);
    if (is_sorted_by_name(lines_arr,
                          lines_arr+number_of_lines)==PASS){
        printf("TEST 5 PASSED\n");}
    else{printf("TEST 5 FAILED\n");}
    if (is_equal(lines_arr, lines_arr+number_of_lines,
                 original_array,
                 original_array+number_of_lines) == PASS){
        printf("TEST 6 PASSED\n");}
    else{printf("TEST 6 FAILED\n");}
    free(original_array);
    free(lines_arr);
}

/**
 * this function recieves the standart input from the use r and checks it
 * validity
 * @param mode an enum for the requested sort (or test)
 * @param error an enum fot the current error value (if any error was detected)
 * @param buffer a buffer to recieve the standart input
 * @param number_of_lines the number of bus lines requested by the user
 * @param lines_arr the lines of the users input
 * @param i running index
 */
void create_line_array(enum SortType mode, int error,
         char *buffer, int number_of_lines,
                           BusLine *lines_arr, int i) {
    while ( i < number_of_lines) {
        printf("Enter line info. Then enter\n");
        char line_name[ARG_LENGTH];
        int distance;
        int duration;
        fgets(buffer, BUFFER_SIZE, stdin);
        sscanf(buffer,"%[^,], %d, %d\n", line_name, &distance,
               &duration);
        char * message = "no error\n";
        int j = 0;
        while (line_name[j] != '\0'){
            if(!(DIGIT_LOWER_BOUND<=line_name[j] &&
            line_name[j]<=DIGIT_UPPER_BOUND)
            && !(LOWERCASE_LOWER_BOUND<= line_name[j] &&
            line_name[j]<=LOWERCASE_UPPER_BOUND)){
                error = INVALID_LINE_NAME;
                message =
                    "bus name should contain only digits and small chars\n";
                break;
                
            }
            j++;
        }
        if (error == NO_ERROR && (distance<MINIMUM_DISTANCE || distance>MAXIMUM_DISTANCE)){
            error = INVALID_DISTANCE;
            message = "distance should be an integer between"
                      " 0 and 100 (includes)\n";
        }
        if(error == NO_ERROR && (duration< MINIMUM_DURATION ||
        duration > MAXIMUM_DURATION)){
            error = INVALID_DURATION;
            message = "duration should be an integer between 10 and 100\n";
        }
        if (error != NO_ERROR){
            printf("ERROR: %s", message);
            error = NO_ERROR;
            create_line_array(mode,error,buffer,number_of_lines,
                       lines_arr, i);
            break;
        }
        struct BusLine line;
        strcpy(line.name, line_name);
        line.distance = distance;
        line.duration = duration;
        lines_arr[i] = line;
        i++;
    }
}

/**
 * this is the main function that recieves as an argument a field to sort by
 * and generate a sorted array as the users choice
 * @param argc number of arguments
 * @param argv should be only the value to sort by
 * @return
 */
int main(int argc, char *argv[]) {

    if (argc == 2) {
        int error = NO_ERROR;

        char buffer[BUFFER_SIZE];
        printf("Enter number of lines. Then enter\n");
        fgets(buffer, BUFFER_SIZE, stdin);

        int number_of_lines = strtol(buffer, NULL,
                                     TEN_BASE);
        if (number_of_lines <= 0 ){
            error = INVALID_NUM_OF_LINES;
        }
        if (error == NO_ERROR) {
            enum SortType mode = get_sort_type(argv);
            BusLine * lines_array =
                    malloc(number_of_lines*sizeof (BusLine));
            if (lines_array == NULL){
                free(lines_array);
                EXIT_FAILURE;
            }

            sort_lines(mode,error,buffer,number_of_lines,
                       lines_array);
            if (mode != TEST){
                for(int  i = 0; i < number_of_lines; i++){
                    printf("%s,%d,%d\n", lines_array[i].name,lines_array[i].distance, lines_array[i].duration);
                    }
                free(lines_array);
            }
        } else{
            printf("ERROR: number of lines"
                   " should be an integer greater than 0\n");
            main(argc,argv);
        }
    }
    else{
        EXIT_FAILURE;
    }
}
