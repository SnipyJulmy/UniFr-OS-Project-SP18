//
// Created by snipy on 28.03.18.
//

#ifndef KEY_VALUE_DATABASE_STACK_H
#define KEY_VALUE_DATABASE_STACK_H


typedef struct Stack Stack;
struct Stack
{
    /* Attributes */
    int* data;
    int size;
    int max_size;
    size_t elt_ptr_size;

    /* Methods */
    void (* destroy)(Stack* self);
    void (* push)(Stack* s, int element);
    int (* pop)(Stack* s);
    int (* peek)(Stack* s);
};

Stack* allocator_stack_create(int init_size);


#endif //KEY_VALUE_DATABASE_STACK_H
