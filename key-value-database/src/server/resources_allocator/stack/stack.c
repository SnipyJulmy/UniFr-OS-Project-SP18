//
// Created by snipy on 28.03.18.
//

#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "../../../debug.h"

#define ALLOCATOR_STACK_INIT_SIZE 100

static void stack_destroy(Stack* self);
static void stack_push(Stack* self, int element);
static int stack_pop(Stack* self);
static int stack_peek(Stack* self);
static void stack_double_size(Stack* stack);

// if init_size <= 0, then init_size would be ALLOCATOR_STACK_INIT_SIZE
Stack* allocator_stack_create(int init_size)
{
    init_size = init_size >= 0 ? init_size : ALLOCATOR_STACK_INIT_SIZE;
    debug("stack -- create a stack of size %d\n", init_size);
    Stack* stack = malloc(1 * sizeof(Stack));
    stack->elt_ptr_size = sizeof(int);
    stack->data = malloc(init_size * stack->elt_ptr_size);
    stack->size = 0;
    stack->max_size = init_size;
    // bind methods
    stack->destroy = stack_destroy;
    stack->peek = stack_peek;
    stack->pop = stack_pop;
    stack->push = stack_push;
    return stack;
}

// private

static void stack_double_size(Stack* stack)
{
    debug("stack -- doubling the size of stack at %u\n", stack);
    stack->max_size = stack->max_size * 2;
    int* data = stack->data;
    stack->data = malloc(stack->max_size * stack->elt_ptr_size);
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

static void stack_push(Stack* self, int element)
{
    if (self->size >= self->max_size)
    {
        stack_double_size(self);
    }
    self->data[self->size] = element;
    self->size += 1;
}

static int stack_pop(Stack* self)
{
    assert(self->size > 0);
    self->size -= 1;
    return self->data[self->size];
}

static int stack_peek(Stack* self)
{
    assert(self->size > 0);
    return self->data[self->size - 1];
}
