//
// Created by snipy on 18.04.18.
//

#ifndef KEY_VALUE_DATABASE_DEQUEUE_H
#define KEY_VALUE_DATABASE_DEQUEUE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Dequeue Dequeue;
typedef struct DequeueNode DequeueNode;

struct Dequeue
{
    /* Attributes */
    int size;
    size_t elt_ptr_size;
    DequeueNode* first;
    DequeueNode* last;

    /* Methods */
    void (* add)(Dequeue*, void*);
    void (* remove)(Dequeue*, void*);
    bool (* contains)(Dequeue*, void*);
    void (* display)(Dequeue*);
    bool (* is_empty)(Dequeue*);
    void (* destroy)(Dequeue*);
    Dequeue* (* clone)(Dequeue*);
    void (* add_first)(Dequeue*, void* item);
    void (* add_last)(Dequeue*, void* item);
    void* (* remove_first)(Dequeue*);
    void* (* remove_last)(Dequeue*);
    void (* move_n_first_to_front)(Dequeue*, Dequeue*, int);
    void (* move_n_last_to_front)(Dequeue*, Dequeue*, int);
    void (* move_n_first_to_end)(Dequeue*, Dequeue*, int);
    void (* move_n_last_to_end)(Dequeue*, Dequeue*, int);
    void* (* peek_first)(Dequeue*);
    void* (* peek_last)(Dequeue*);

    /* Item related Methods */
    int (* item_compare)(void*, void*);
    void (* item_destroy)(void*);
};

struct DequeueNode
{
    void* data;
    DequeueNode* prev;
    DequeueNode* next;
};

Dequeue* key_value_database_dequeue_create(size_t elt_ptr_size,
                                           int (* fn_item_compare)(void*, void*),
                                           void (* fn_item_destroy)(void*))

#endif //KEY_VALUE_DATABASE_DEQUEUE_H
