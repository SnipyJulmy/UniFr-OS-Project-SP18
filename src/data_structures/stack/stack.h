//
// Created by snipy on 28.03.18.
//

#ifndef KEY_VALUE_DATABASE_STACK_H
#define KEY_VALUE_DATABASE_STACK_H


typedef struct Stack Stack;
typedef struct StackNode StackNode;

struct Stack
{
    /* Attributes */
    void** data;
    int size;
    int max_size;
    size_t elt_ptr_size;

    /* Methods */
    void (* destroy)(Stack* self);
    void (* push)(Stack* self, void* element);
    void* (* pop)(Stack* self);
    void* (* peek)(Stack* self);
    bool (* is_empty)(Stack* self);
};

struct StackNode
{
    bool is_free;
    void* data;
};

Stack* allocator_stack_create(int init_size, size_t elt_size);

#endif //KEY_VALUE_DATABASE_STACK_H
