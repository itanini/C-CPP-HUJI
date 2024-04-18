#include "cipher.h"
#include "tests.h"



#include "cipher.h"
#include "tests.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define CIPHER_ARGS 5
#define TEST_ARGS 2
#define RADIX 10
#define BUFFER_LENGTH 1024
#define TRUE 1
#define ENCODE 11
#define DECODE 12

// your code goes here

/**
 * checks if the given string k starts with a negative sign.
 *
 * @param k the string to check
 * @return 1 if k starts with a negative sign, 0 otherwise
 */
int is_negative(char const *k){
    if (k[0] == '-'){
        return 1;
    }
    else{
        return 0;
    }
}

/**
 * Checks if the given string k represents a valid integer value.
 *
 * @param k the string to check
 * @return 1 if k represents a valid integer value, 0 otherwise
 */
int is_valid_k(char  const *k){
    int k_length = strlen(k);
    for (int i = is_negative(k); i < k_length; i++){
        if (!isdigit(k[i])){
            return 0;
        }
    }
    return TRUE;
}

/**
 * Checks the command argument and returns the corresponding command value.
 *
 * @param command_arg the command argument to check
 * @return the command value corresponding to the command argument, or 0 if
 * the command argument is invalid
 */
int command_check(char *command_arg){
    if (!strcmp(command_arg, "encode")){
        return ENCODE;
    }
    else if (!strcmp(command_arg, "decode")){
        return DECODE;
    }
    return 0;
}

int illegalFiles(FILE *input_text, FILE *output_text) {
    if(input_text == NULL && output_text == NULL) {
        fprintf(stderr, "The given file is invalid.\n");
        return EXIT_FAILURE;
    }
    else if(input_text == NULL){
        fprintf(stderr, "The given file is invalid.\n");
        fclose(output_text);
        return EXIT_FAILURE;
    }
    else if(output_text == NULL){
        fprintf(stderr, "The given file is invalid.\n");
        fclose(input_text);
        return EXIT_FAILURE;
    }
    return 0;
}

// Function to encode or decode the file
void allLegal(long k, char *buffer, int command, FILE *input_file, FILE
*output_file) {
    while (fgets(buffer, BUFFER_LENGTH, input_file)
    != NULL) {
        if (command == ENCODE) {
            encode(buffer, (int) k);
            fprintf(output_file, "%s", buffer);
        }
        if (command == DECODE) {
            decode(buffer, (int) k);
            fprintf(output_file, "%s", buffer);
        }
    }
}


int tests_check(){
    if (!test_decode_cyclic_lower_case_negative_k()
    &&
    !test_decode_cyclic_lower_case_special_char_positive_k()
    &&
    !test_decode_non_cyclic_lower_case_special_char_negative_k()
    &&
    !test_decode_cyclic_upper_case_positive_k()
    &&
    !test_decode_non_cyclic_lower_case_positive_k()
    &&
    !test_encode_cyclic_lower_case_negative_k()
    &&
    !test_encode_non_cyclic_lower_case_special_char_negative_k()
    &&
    !test_encode_cyclic_lower_case_special_char_positive_k()
    &&
    !test_encode_cyclic_upper_case_positive_k()
    &&
    !test_encode_non_cyclic_lower_case_positive_k())
    {
        return TRUE;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == CIPHER_ARGS) { // If the number of arguments is correct for
        // the cipher mode
        // Save arguments
        char *command_arg = argv[1];
        long k = strtol(argv[2], NULL, RADIX);
        char *i_path = argv[3];
        char *o_path = argv[4];

        char buffer[BUFFER_LENGTH] = "";
        int command = command_check(command_arg); // Check if command
        // argument is valid
        if (command) { // If command is valid
            if (!is_valid_k(argv[2])) { // Check if k is valid
                fprintf(stderr, "The given shift value"
                                " is invalid.\n");
                return EXIT_FAILURE;
            }

            // Open input and output files and check validity
            FILE *input_file = fopen(i_path, "r");
            FILE *output_file = fopen(o_path, "w");
            if (illegalFiles(input_file, output_file)) {
                return EXIT_FAILURE;
            }

            // Call function to encode or decode the file
            allLegal(k, buffer, command, input_file, output_file);

            // Close files
            fclose(input_file);
            fclose(output_file);
            return EXIT_SUCCESS;
        }

        // If command is not valid
        fprintf(stderr, "The given command is invalid.\n");
        return EXIT_FAILURE;
    }
    else if (argc == TEST_ARGS) { // If the number of arguments is correct
        // for the test mode
        char *command_arg = argv[1];
        if (!strcmp(command_arg, "test")) { // If the command is to run tests
            if (tests_check()) { // If tests pass
                return EXIT_SUCCESS;
            }
            return EXIT_FAILURE;
        }
        else { // If command is not to run tests
            fprintf(stderr, "Usage: cipher test\n");
            return EXIT_FAILURE;
        }
    }
    else { // If the number of arguments is incorrect
        fprintf(stderr, "The program receives 1 or 4"
                        " arguments only.\n");
        return EXIT_FAILURE;
    }
}

