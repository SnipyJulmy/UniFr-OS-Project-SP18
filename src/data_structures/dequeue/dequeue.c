//
// Created by snipy on 18.04.18.
//

#include <assert.h>
#include <stdlib.h>

#include "dequeue.h"
#include "../../debug.h"

/************************************************************************************/
/*                                START PRIVATE DECLARATION                         */
/************************************************************************************/

/**
 * Add an element at the end of the List list
 * Assume that the element is not present in the list
 */
static void __add(Dequeue* dequeue, void* data);

/**
 *  Add a element at the front of the list
 */
static void __add_first(Dequeue* self, void* data);

/**
 *  Add a element at the end of the list
 */
static void __add_last(Dequeue* self, void* data);

/**
 *  Remove the exact element from the dequeue
 */
static void __remove(Dequeue* self, void* data);

/**
 *  Remove the first element
 */
static void* __remove_first(Dequeue* self);

/**
 *  Remove the last element
 */
static void* __remove_last(Dequeue* self);

/**
 *  move the first n element from src and append them to the front of dst
 */
static void __move_n_first_to_front(Dequeue* src, Dequeue* dst, int n);

/**
 *  move the last n element from src and append them to the front of dst
 */
static void __move_n_last_to_front(Dequeue* src, Dequeue* dst, int n);

/**
 *  move the first n element from src and append them to the end of dst
 */
static void __move_n_first_to_end(Dequeue* src, Dequeue* dst, int n);

/**
 *  move the last n element from src and append them to the end of dst
 */
static void __move_n_last_to_end(Dequeue* src, Dequeue* dst, int n);

/**
 * Return the first element without removing him
 */
static void* __peek_first(Dequeue* self);

/**
 * Return the last element without removing him
 */
static void* __peek_last(Dequeue* self);

/**
 * Test if the exact element is present
 */
static bool __contains(Dequeue* dequeue, void* data);

/**
 * Test if the list is empty or not
 * true --> the list is empty
 */
static bool __is_empty(Dequeue* dequeue);

/**
 * Execute a function for each element in the dequeue
 * function f is of type void* -> void like in most functional programming language
 */
static void __foreach(Dequeue* dequeue, void(* f)(void*));

/**
 * Free a list but do not free
 * The matrix that has been added to
 */
static void __destroy(Dequeue* self);

/**
 * Display the dequeue... for debug
 */
static void __display(Dequeue* self);

/************************************************************************************/
/*                                  END DECLARATION                                 */
/************************************************************************************/

Dequeue* key_value_database_dequeue_create(size_t elt_ptr_size,
                                           int (* fn_item_compare)(void*, void*),
                                           void (* fn_item_destroy)(void*))
{
    Dequeue* res = malloc(1 * sizeof(Dequeue));
    res->size = 0;
    res->first = malloc(1 * sizeof(DequeueNode));   // sentinel
    check_mem_and_return(res->first, NULL);
    res->last = malloc(1 * sizeof(DequeueNode));   // sentinel
    check_mem(res->last, {
        free(res->first);
        return NULL;
    });

    res->first->next = res->last;
    res->first->prev = NULL;
    res->last->prev = res->first;
    res->last->next = NULL;

    res->add = __add;
    res->remove = __remove;
    res->contains = __contains;
    res->destroy = __destroy;
    res->is_empty = __is_empty;
    res->add_first = __add_first;
    res->add_last = __add_last;
    res->remove_first = __remove_first;
    res->remove_last = __remove_last;
    res->move_n_first_to_front = __move_n_first_to_front;
    res->move_n_first_to_end = __move_n_first_to_end;
    res->move_n_last_to_front = __move_n_last_to_front;
    res->move_n_last_to_end = __move_n_last_to_end;
    res->peek_first = __peek_first;
    res->peek_last = __peek_last;
    res->display = __display;
    res->foreach = __foreach;

    res->elt_ptr_size = elt_ptr_size;
    res->item_compare = fn_item_compare;
    res->item_destroy = fn_item_destroy;

    return res;
}

static void __add(Dequeue* dequeue, void* data)
{
    DequeueNode* penultimate = dequeue->last->prev;
    DequeueNode* node_to_add = malloc(1 * sizeof(DequeueNode));
    penultimate->next = node_to_add;
    dequeue->last->prev = node_to_add;
    node_to_add->prev = penultimate;
    node_to_add->next = dequeue->last;
    node_to_add->data = data;
    dequeue->size += 1;
}

static void __remove(Dequeue* self, void* data)
{
    if (self->size == 0)
    {
        return;
    }
    DequeueNode* crt_elt = self->first->next;
    while (crt_elt != self->last)
    {
        if (self->item_compare(crt_elt->data, data) == 0)
        {
            DequeueNode* prev = crt_elt->prev;
            DequeueNode* next = crt_elt->next;
            prev->next = next;
            next->prev = prev;
            free(crt_elt);
            break;
        }
        crt_elt = crt_elt->next;
    }
    self->size -= 1;
}

static bool __contains(Dequeue* dequeue, void* data)
{
    if (dequeue == NULL || dequeue->size == 0)
    {
        return false;
    }

    DequeueNode* crt_elt = dequeue->first->next;

    while (crt_elt != dequeue->last)
    {
        if (dequeue->item_compare(crt_elt->data, data) == 0)
        {
            return true;
        }
        crt_elt = crt_elt->next;
    }
    return false;
}

static bool __is_empty(Dequeue* dequeue)
{
    return dequeue->size == 0;
}

static void __destroy(Dequeue* self)
{
    DequeueNode* crt_node = self->first->next;
    DequeueNode* tmp;

    while (crt_node != self->last)
    {
        tmp = crt_node->next;
        free(crt_node);
        crt_node = tmp;
    }

    free(self->first);
    free(self->last);
    free(self);
}

static void __add_first(Dequeue* self, void* data)
{
    DequeueNode* second = self->first->next;
    DequeueNode* new_node = malloc(1 * sizeof(DequeueNode));
    new_node->data = data;
    new_node->next = second;
    new_node->prev = self->first;
    self->first->next = new_node;
    second->prev = new_node;
    self->size++;
}

static void __add_last(Dequeue* self, void* data)
{
    DequeueNode* penultimate = self->last->prev;
    DequeueNode* new_node = malloc(1 * sizeof(DequeueNode));
    new_node->data = data;
    new_node->next = self->last;
    new_node->prev = penultimate;
    penultimate->next = new_node;
    self->last->prev = new_node;
    self->size++;
}

static void* __remove_first(Dequeue* self)
{
    assert(self->size >= 0);
    if (self->size == 0)
    {
        return NULL;
    }
    void* res = self->first->next->data;
    DequeueNode* third = self->first->next->next;
    DequeueNode* second = self->first->next;
    self->first->next = third;
    third->prev = self->first;
    free(second);
    self->size--;
    return res;
}

static void* __remove_last(Dequeue* self)
{
    assert(self->size >= 0);
    if (self->size == 0)
    {
        return NULL;
    }
    void* res = self->last->prev->data;
    DequeueNode* penultimate = self->last->prev;
    DequeueNode* penulpenultimate = self->last->prev->prev;
    self->last->prev = penulpenultimate;
    penulpenultimate->next = self->last;
    free(penultimate);
    self->size--;
    return res;
}

static void __move_n_first_to_front(Dequeue* src, Dequeue* dst, int n)
{
    DequeueNode* nth_node = src->first->next;
    int itr = n;
    while (itr)
    {
        if (nth_node == src->last)
        {
            break;
        }
        nth_node = nth_node->next;
        itr--;
    }
    if (n == itr)
    {
        return;
    }
    DequeueNode* dst_second = dst->first->next;

    dst->first->next = src->first->next;
    dst->first->next->prev = dst->first;
    dst_second->prev = nth_node->prev;
    nth_node->prev->next = dst_second;
    src->first->next = nth_node;
    nth_node->prev = src->first;

    src->size -= (n - itr);
    dst->size += (n - itr);
}

static void __move_n_last_to_front(Dequeue* src, Dequeue* dst, int n)
{
    DequeueNode* nth_from_end_node = src->last->prev;
    int itr = n;
    while (itr)
    {
        if (nth_from_end_node == src->first)
        {
            break;
        }
        nth_from_end_node = nth_from_end_node->prev;
        itr--;
    }
    if (n == itr)
    {
        return;
    }
    DequeueNode* dst_second = dst->first->next;

    dst->first->next = nth_from_end_node->next;
    dst->first->next->prev = dst->first;
    dst_second->prev = src->last->prev;
    dst_second->prev->next = dst_second;
    nth_from_end_node->next = src->last;
    src->last->prev = nth_from_end_node;

    src->size -= (n - itr);
    dst->size += (n - itr);
}

static void __move_n_last_to_end(Dequeue* src, Dequeue* dst, int n)
{
    DequeueNode* nth_from_end_node = src->last->prev;
    int itr = n;
    while (itr)
    {
        if (nth_from_end_node == src->first)
        {
            break;
        }
        nth_from_end_node = nth_from_end_node->prev;
        itr--;
    }
    if (n == itr)
    {
        return;
    }
    DequeueNode* dst_penultimate = dst->last->prev;

    dst_penultimate->next = nth_from_end_node->next;
    dst_penultimate->next->prev = dst_penultimate;
    dst->last->prev = src->last->prev;
    dst->last->prev->next = dst->last;

    nth_from_end_node->next = src->last;
    src->last->prev = nth_from_end_node;

    src->size -= (n - itr);
    dst->size += (n - itr);
}

static void __move_n_first_to_end(Dequeue* src, Dequeue* dst, int n)
{
    DequeueNode* nth_node = src->first->next;
    int itr = n;
    while (itr)
    {
        if (nth_node == src->last)
        {
            break;
        }
        nth_node = nth_node->next;
        itr--;
    }
    if (n == itr)
    {
        return;
    }
    DequeueNode* dst_penultimate = dst->last->prev;

    dst_penultimate->next = src->first->next;
    dst_penultimate->next->prev = dst_penultimate;
    dst->last->prev = nth_node->prev;
    dst->last->prev->next = dst->last;

    src->first->next = nth_node;
    nth_node->prev = src->first;

    src->size -= (n - itr);
    dst->size += (n - itr);
}

static void* __peek_first(Dequeue* self)
{
    if (self->size == 0)
    {
        return NULL;
    }
    return self->first->next->data;
}

static void* __peek_last(Dequeue* self)
{
    if (self->size == 0)
    {
        return NULL;
    }
    return self->last->prev->data;
}

void __display(Dequeue* self)
{
    #if !defined(NDEBUG) && (LOG_LEVEL >= LOG_LEVEL_DEBUG)
    DequeueNode* node = self->first;
    while (node->next != self->last)
    {
        node = node->next;
        KV* kv = node->data;
        debug_nl;
        debug("(%u,%s)\n", kv->key, kv->value);
        debug_nl;
    }
    #endif
}

void __foreach(Dequeue* dequeue, void (* f)(void*))
{
    DequeueNode* crt = dequeue->first;
    while (crt->next != dequeue->last)
    {
        f(crt->data);
    }
}
