//
// Created by snipy on 28.03.18.
//

#include "key_value_database_allocator.h"
#include "stack/stack.h"
#include "../../debug.h"

static Stack* stack;

// when we allocate memory, we push it on the stack
// so we just have to free the whole stack when
// exiting the application
K* key_value_database_allocate(size_t size)
{
    return NULL;
}

// When a particular
void key_value_database_deallocate(K* string)
{

}

void key_value_database_destroy(void)
{
    // free allocate memory which is on the stack
    while (!stack->is_empty(stack))
        free(stack->pop);
    // free the stack itself
    stack->destroy(stack);
}

bool key_value_database_init(int init_stack_size)
{
    stack = allocator_stack_create(init_stack_size, sizeof(K));
    check_mem(stack, {
        log_err("Can't init stack for allocator in __FILE___\n");
        return false;
    });
    return true;
}
