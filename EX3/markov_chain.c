//
// Created by itani on 4/28/2023.
//

#include <string.h>
#include "markov_chain.h"

#define ONE 1

Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr){
    struct Node * node = NULL;
    Node * cur_node  = markov_chain -> database -> first;
    // check over all nodes in the chain and look for a similar name
    for (int i = 0; i < markov_chain -> database -> size; i++){
        char * word_at_i = cur_node->data->data;
        if (!strcmp(word_at_i, data_ptr)){
            return cur_node;
        }
        if (cur_node->next){
            cur_node = cur_node->next;
        }
    }
    //if a identical node was not found returns null
    return node;
}

Node* add_to_database(MarkovChain *markov_chain, char *data_ptr) {
    Node *node = get_node_from_database(markov_chain, data_ptr);
    if (!node) {
        struct MarkovNode *markov_node =
                malloc(sizeof(struct MarkovNode));
        if (!markov_node) {
            free_database(&markov_chain);
            return NULL;
        }
        // Initialize the new MarkovNode struct with the given data_ptr
        // and frequencies_list
        markov_node->data = data_ptr;
        markov_node->frequencies_list = NULL;
        markov_node->freq_list_len = 0;
        markov_node->total_freq = 0;
        // Allocate memory for the new Node struct
        struct Node *new_node = malloc(sizeof(struct Node));
        if (!new_node) {
            // Handle the error if the allocation fails
            free(markov_node);
            free(new_node);
            free_database(&markov_chain);
            return NULL;
        }
        // Initialize the new Node struct with the new MarkovNode struct
        // and NULL for next
        new_node->data = markov_node;
        // starts the next node as null; only next round will assign an
        //actual node
        new_node->next = NULL;
        // Return the pointer to the new Node struct
        if (!(markov_chain->database->last)){
            markov_chain->database->first=new_node;
            markov_chain->database->last=new_node;
            markov_chain->database->size =1;
            return new_node;
        }
        markov_chain->database->last->next = new_node;
        markov_chain->database->last = new_node;
        markov_chain->database->size++;
        return new_node;
    }
    return node;
}



bool add_node_to_frequencies_list(MarkovNode *first_node,
                                  MarkovNode *second_node){
    char * node_data = second_node->data;
    if (first_node->freq_list_len ==0){
        // Allocate memory for the new MarkovNodeFrequency struct
        struct MarkovNodeFrequency *frequencies_list =
                calloc(1,
                       sizeof(struct MarkovNodeFrequency));
        if (!frequencies_list) {
            // Handle the error if the allocation fails
            return false;
        }
        // first appearance will be counted as 0
        struct MarkovNodeFrequency second_markov_node_frequency=
                {second_node, 0};
        frequencies_list[0] = second_markov_node_frequency;
        first_node -> frequencies_list = frequencies_list;
        first_node -> freq_list_len = 1;
        first_node -> total_freq =1;
        return true;
    }
    for (int i = 0;  i < first_node->freq_list_len; i++){
        char * word_at_i = first_node->frequencies_list[i].markov_node->data;
        if (!strcmp(word_at_i, node_data)){
            first_node->frequencies_list[i].frequency++;
            first_node ->total_freq++;
            return true;
        }
    }
    void * tmp =
            realloc(first_node->frequencies_list,
                    (first_node->freq_list_len+1)
                    *sizeof(MarkovNodeFrequency));
    if (!tmp){
        return false;
    }
    first_node->frequencies_list = tmp;
    struct MarkovNodeFrequency second_markov_node_frequency=
            {second_node, 1};
    first_node->frequencies_list[first_node->freq_list_len]=
            second_markov_node_frequency;
    first_node->freq_list_len++;
    first_node->total_freq++;
    return  true;
}

void free_database(MarkovChain ** ptr_chain){
    if (*ptr_chain){
        MarkovChain * mc = *ptr_chain;
        LinkedList * all_nodes = mc->database;
        Node * cur_node = all_nodes->first;
        for (int i = 0 ; i < (all_nodes->size); i++){
            free(cur_node->data->frequencies_list);
            cur_node->data->frequencies_list = NULL;
            //free the name of the node
            free(cur_node->data->data);
            cur_node->data->data = NULL;
            free(cur_node->data);
            cur_node->data =NULL;
            Node * node_to_free= cur_node;
            cur_node = cur_node->next;
            free(node_to_free);
            node_to_free = NULL;
        }
        free(cur_node);
        free(all_nodes);
        free(mc);
    }
}

int get_random_number(int max_number){
    return rand() % max_number;
}

MarkovNode * get_first_random_node(MarkovChain * markov_chain){
    int rand_index = get_random_number
            (markov_chain->database->size);
    Node * cur_node = markov_chain->database->first;
    for (int i =0 ; i<rand_index; i++){
        cur_node = cur_node->next;
    }
    if (cur_node->next) {
        if (cur_node->data->frequencies_list) {
            return cur_node->data;
        }
    }
    return  NULL;
}

MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr){
    int rand_number = get_random_number
            (state_struct_ptr->total_freq);
    int i = 0;
    int i_freq = state_struct_ptr ->frequencies_list[i].frequency;
    //runs on the nodes by the order. for each frequency there is another
    // chance to hit that node
    while (rand_number - i_freq > 0){
        rand_number -= i_freq;
        i++;
        i_freq = state_struct_ptr->frequencies_list[i].frequency;
    }
    return state_struct_ptr->frequencies_list[i].markov_node;
}

