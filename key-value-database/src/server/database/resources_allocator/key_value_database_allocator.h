//
// Created by snipy on 28.03.18.
//

#ifndef KEY_VALUE_DATABASE_ALLOCATOR_INT_BOX_H
#define KEY_VALUE_DATABASE_ALLOCATOR_INT_BOX_H

#include <stdlib.h>
#include <stdbool.h>
#include "key_value_database_typedef.h"

// init the value memory allocator
bool key_value_database_init(int init_stack_size);
// allocate a string on the stack
Key* key_value_database_allocate(size_t size);
// deallocate the string
void key_value_database_deallocate(Key* key);
// free the whole structure
void key_value_database_destroy(void);

#endif //KEY_VALUE_DATABASE_ALLOCATOR_INT_BOX_H
