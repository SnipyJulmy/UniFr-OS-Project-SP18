//
// Created by snipy on 28.03.18.
//

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "stack.h"
#include "../../../../debug.h"

#define ALLOCATOR_STACK_INIT_SIZE 100

static void stack_destroy(Stack* self);
static void stack_push(Stack* self, void* element);
static void* stack_pop(Stack* self);
static void* stack_peek(Stack* self);
static void stack_double_size(Stack* stack);
static bool stack_is_empty(Stack* stack);

// if init_size <= 0, then init_size would be ALLOCATOR_STACK_INIT_SIZE
Stack* allocator_stack_create(int init_size, size_t elt_size)
{
    init_size = init_size >= 0 ? init_size : ALLOCATOR_STACK_INIT_SIZE;
    debug("stack -- create a stack of size %d\n", init_size);
    Stack* stack = malloc(1 * sizeof(Stack));
    check_mem(stack, {
        log_err("Can't allocate memory for a stack in __FILE__ on line __LINE__\n");
        exit(EXIT_FAILURE);
    });
    stack->elt_ptr_size = elt_size;
    stack->data = malloc(init_size * stack->elt_ptr_size);
    stack->size = 0;
    stack->max_size = init_size;
    // bind methods
    stack->destroy = stack_destroy;
    stack->peek = stack_peek;
    stack->pop = stack_pop;
    stack->push = stack_push;
    stack->is_empty = stack_is_empty;
    return stack;
}

// private

static void stack_double_size(Stack* stack)
{
    debug("stack -- doubling the size of stack at %u\n", stack);
    stack->max_size = stack->max_size * 2;
    void** data = stack->data;
    stack->data = malloc(stack->max_size * stack->elt_ptr_size);
    check_mem(stack->data, {
        log_err("Can't double the size of the stack in __FILE__ on line __LINE__\n");
        exit(EXIT_FAILURE);
    });
    for (int i = 0; i < stack->max_size / 2; i++)
    {
        stack->data[i] = data[i];
    }
    free(data);
}

static void stack_destroy(Stack* self)
{
    free(self->data);
    free(self);
}

static void stack_push(Stack* self, void* element)
{
    if (self->size >= self->max_size)
    {
        stack_double_size(self);
    }
    self->data[self->size] = element;
    self->size += 1;
}

static void* stack_pop(Stack* self)
{
    assert(self->size > 0);
    self->size -= 1;
    return self->data[self->size];
}

static void* stack_peek(Stack* self)
{
    assert(self->size > 0);
    return self->data[self->size - 1];
}

bool stack_is_empty(Stack* stack)
{
    return stack->size == 0;
}
