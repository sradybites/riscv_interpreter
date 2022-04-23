#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

struct linkedlist
{
    struct linkedlist_node *first;
    // TODO: define linked list metadata
    int length;
};

struct linkedlist_node
{
    // TODO: define the linked list node
    int key;
    int value;
    struct linkedlist_node *next;
};
typedef struct linkedlist_node linkedlist_node_t;

linkedlist_t *ll_init()
{
    // TODO: create a new linked list

    // We have done this TODO for you as an example of how to use malloc().
    // You might want to read more about malloc() from Linux Manual page.
    // Usually free() should be used together with malloc(). For example,
    // the linkedlist you are currently implementing usually have free() in the
    // ll_free() function.

    // First, you use C's sizeof function to determine
    // the size of the linkedlist_t data type in bytes.
    // Then, you use malloc to set aside that amount of space in memory.
    // malloc returns a void pointer to the memory you just allocated, but
    // we can assign this pointer to the specific type we know we created
    linkedlist_t *list = malloc(sizeof(linkedlist_t));
    list->first = NULL;

    // TODO: set metadata for your new list and return the new list
    list->length = 0;
    return list;
}

void ll_free(linkedlist_t *list)
{
    // TODO: free a linked list from memory
    linkedlist_node_t *curr = list->first;
    while(curr != NULL) { // frees all nodes until null ptr reached
        linkedlist_node_t *tmp = curr->next;
        free(curr);
        curr = tmp;
    }
    free(list);
}

void ll_add(linkedlist_t *list, int key, int value)
{
    // TODO: create a new node and add to the front of the linked list if a
    // node with the key does not already exist.
    // Otherwise, replace the existing value with the new value.
    linkedlist_node_t *curr = list->first;
    linkedlist_node_t *prev = NULL;
    while(curr != NULL && curr->key != key) { // scan list for desired node
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL) { // node do not exist
        linkedlist_node_t *new = malloc(sizeof(linkedlist_node_t));
        new->key = key;
        new->value = value;
        new->next = NULL;

        if(prev == NULL) {
            list->first = new;
        } else {
            prev->next = new;
        }
        list->length++;
    } else {
        curr->value = value;
    }
}

int ll_get(linkedlist_t *list, int key)
{
    // TODO: go through each node in the linked list and return the value of
    // the node with a matching key.
    // If it does not exist, return 0.
    linkedlist_node_t *curr = list->first;
    while(curr != NULL && curr->key != key) {
        curr = curr->next;
    }
    return curr == NULL ? 0 : curr->value;
}

int ll_size(linkedlist_t *list)
{
    // TODO: return the number of nodes in this linked list
    return list->length;
}
