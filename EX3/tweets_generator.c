//
// Created by itani on 4/28/2023.
//

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "markov_chain.h"

#define BASE 10
#define MAX_WORD 100
#define BUFFER_SIZE 1000
#define ALL -1


void usage_error();

int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain);

MarkovChain *create_markov_chain();

Node *set_prv(bool dot, Node *last_node);

bool check_dot(const char *token);

/**
 * prints tweets randomlly by the chances of one word to foolow the prior one
 * @param argc  num of args +1
 * @param argv The program expect to
           receive 3 or 4 parameters:
           1: int seed, 2: int number of tweets to generate,
           3: char * path to the learning file,
           4: int number of word to read from the file
 * @return
 */
    int main(int argc, char *argv[]) {

        if (argc == FIVE || argc == FOUR) {
            int seed = strtol(argv[ONE], NULL, BASE);
            int num_of_tweets = strtol(argv[2], NULL, BASE);
            char *path = argv[3];
            int read_num;
            if (argc == FIVE) {
                read_num = strtol(argv[FOUR], NULL, BASE);
            } else{
                read_num = ALL;
            }
            srand(seed);
            FILE *file = fopen(path, "r");
            if (!file) {
                fprintf(stderr, "Error: failed to open file");
                return EXIT_FAILURE;
            }
            MarkovChain *markov_chain = create_markov_chain();
            if (!markov_chain) {
                free_database(&markov_chain);
                return EXIT_FAILURE;
            }
            if (fill_database(file,
                              read_num, markov_chain) == ONE) {
                free_database(&markov_chain);
                return EXIT_FAILURE;
            }
            for (int i = 0; i < num_of_tweets; i++) {
                printf( "Tweet %d: ", i+1);
                generate_tweet(markov_chain,
                               get_first_random_node(markov_chain),
                               MAX_TWEET_LENGTH);
            }
            free_database(&markov_chain);
        }
        else{
            usage_error();
            return EXIT_FAILURE;
        }
}


/**
 * intialize a makov chan and set a database to it
 * @return a new virgin markov chain
 */
MarkovChain *create_markov_chain() {
    MarkovChain * markov_chain = malloc(sizeof(MarkovChain));
    if (!markov_chain){
        return NULL;
    }
    markov_chain->database = calloc
            (ONE, sizeof(LinkedList));
    if (!(markov_chain->database)){
        free(markov_chain);
        return NULL;
    }

    return markov_chain;
}

/**
 * this function gets the data and adds nodes to the markov chain
 * @param fp the file path
 * @param words_to_read number of words to read from the data and put in
 * the markov chain
 * @param markov_chain a pointer to the chain itself
 * @return returns 0 if succeeded 1 if failed
 */
int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain) {
    char buffer[BUFFER_SIZE];
    while (fgets (buffer, BUFFER_SIZE, fp)
    &&
            (words_to_read == ALL ||
            markov_chain->database->size < words_to_read)){
        char * token = strtok(buffer, " \n\t\r");
        bool dot =false;
        Node * prv_node = NULL;
        while (token != NULL && (words_to_read == ALL ||
        markov_chain->database->size < words_to_read)){
            dot = check_dot(token);
            char * token_buf = NULL;
            Node * repeating_node = get_node_from_database(markov_chain,
                                                           token);
            if (!repeating_node) {
                token_buf = malloc(MAX_WORD * sizeof(char));
                if (!token_buf) {
                    free(token_buf);
                    return ONE;}
                strcpy(token_buf, token);}
            else{
                token_buf = repeating_node->data->data;
            }
            Node * last_node = add_to_database(markov_chain,token_buf);
            if (!last_node){
                free(token_buf);
                return ONE;
            }
            if (prv_node){
                if(!add_node_to_frequencies_list(prv_node ->data,
                                             last_node -> data)) {
                    free(token_buf);
                    return ONE;}
            }
            prv_node = set_prv(dot, last_node);
            token = strtok(NULL, " \n\t\r");
            dot = false;}
    }
    return 0;
}

/**
 * check if the new word was ended with a dot
 * @param token new token
 * @return true if was ended with a dot
 */
bool check_dot(const char *token) {
    bool dot;
    int token_len = strlen(token);
    if (token[token_len-ONE] == '.'){
        dot = true;}
    return dot;
}

/**
 * sets the previous node to be the new added node, so we can set the
 * next node to be added as its data->next/
 * @param dot is the last node was ended with a '.'
 * @param last_node the last produced node
 * @return
 */
Node *set_prv(bool dot, Node *last_node) {
    Node *prv_node;
    if (dot){
        prv_node = NULL;
    }
    else {
        prv_node = last_node;
    }
    return prv_node;
}

void usage_error() {
    printf("Usage: The program expect to "
           "receive 3 or 4 parameters:"
           "1: int seed, 2: int number of tweets to generate,"
           " 3: char * path to the learning file,"
           "4: int number of word to read from the file"
           " (optional [default = read all] ");
}

void generate_tweet(MarkovChain *markov_chain,
                    MarkovNode *first_node, int max_length){
        while (!first_node){
            first_node = get_first_random_node(markov_chain);
        }
        int i = 0;
        while (first_node->data[strlen(first_node->data)-ONE] != '.' &&
        i < max_length-ONE){
            printf("%s ", first_node->data);
            first_node = get_next_random_node(first_node);
            i++;
        }
        printf("%s\n", first_node->data);
    }


