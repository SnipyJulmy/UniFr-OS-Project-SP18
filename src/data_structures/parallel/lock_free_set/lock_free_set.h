#ifndef LOCK_FREE_SET_LOCK_FREE_SET_H
#define LOCK_FREE_SET_LOCK_FREE_SET_H

#include <stdbool.h>
#include <stdint.h>
#include "../../../server/database/key_value_database_typedef.h"
#include "../../dequeue/dequeue.h"

#define MAX_LOAD 2
#define INIT_SET_SIZE 2

typedef struct Node Node;
struct Node
{
    bool sentinel;
    uint32_t reversed_key;
    void* data;
    Node* next;
};

typedef struct Set Set;
struct Set
{
    /* attributes */
    Node*** first_bucket;
    uint32_t item_count;
    uint32_t size;

    /* methods */
    // free the whole set recursively
    void (* destroy)(Set* self);
    bool (* add)(Set* self, void* data);
    bool (* add_with_key)(Set* self, uint32_t key, void* data);
    bool (* remove)(Set* self, void* data);
    bool (* contains_from_value)(Set* self, void* data);
    bool (* contains_from_key)(Set* self, uint32_t key);
    bool (* update)(Set* self, uint32_t key, void* data);
    bool (* remove_from_key)(Set* self, uint32_t key);
    bool (* remove_from_value)(Set* self, void* data); // only works when data is a char**
    void* (* read)(Set* self, uint32_t key);
    uint32_t (* read_key_from_value)(Set* self, void* data); // only works when data is a char**
    Dequeue* (* ls)(Set* self);

    // private
    uint32_t (* item_hashcode)(void*);  // hashcode for the item to store

    // for the dequeue
    int (* dequeue_item_compare)(void*, void*);  // to compare two item
    void (* dequeue_item_destroy)(void*); // to free an item in the dequeue
};

// use in order to manipulate the pointer value without
// compiler error
typedef union
{
    Node* node;
    int64_t value;
} Conversion;

Set* lock_free_data_create_set(uint32_t (* fn_hashcode)(void*));
uint32_t lock_free_data_reverse(uint32_t i);

#endif //LOCK_FREE_SET_LOCK_FREE_SET_H