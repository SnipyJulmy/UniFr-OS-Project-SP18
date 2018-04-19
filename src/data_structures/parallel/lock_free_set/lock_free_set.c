//
// Created by snipy on 14.03.18.
//

#include <stdlib.h>
#include <stdio.h>

#include "lock_free_set.h"
#include "../../../debug.h"
#include "../atomic.h"

// private function

static Node* lock_free_data_initialize_bucket(Set* self, uint32_t key);
static Node* lock_free_data_get_secondary_bucket(Set* self, uint32_t key);
static bool lock_free_data_list_insert(Node* head, Node* node);
static bool lock_free_data_find(Node* head, uint32_t reversed_key, bool is_sentinel, Node** pred, Node** current);
static void recursive_free(Node* node);

/***********/
/* methods */
/***********/

// add
bool lock_free_data_add(Set* self, void* data);
bool lock_free_data_add_with_key(Set* self, uint32_t key, void* data);

// remove
bool lock_free_data_remove(Set* self, void* data);
bool lock_free_data_remove_from_key(Set* self, uint32_t key);

// contains
bool lock_free_data_contains(Set* self, void* data);
bool lock_free_data_contains_from_key(Set* self, uint32_t key);

// read
void* lock_free_data_read(Set* self, Key key);

// ls
Dequeue* lock_free_ls(Set* set);

// utility methods
void lock_free_data_free(Set* self);

/**
 *  Create a new lock-free set structure
 *  @return a pointer of the freshly created lock-free hashset or NULL if an error occurs
 */
Set* lock_free_data_create_set(uint32_t (* fn_hashcode)(void*))
{
    Set* set = malloc(1 * sizeof(Set));
    check_mem(set,
              return NULL);

    set->size = INIT_SET_SIZE;
    set->item_count = 0;

    // TODO initialize all the methods
    // Methods binding
    set->add = lock_free_data_add;
    set->add_with_key = lock_free_data_add_with_key;
    set->contains = lock_free_data_contains;
    set->remove = lock_free_data_remove;
    set->item_hashcode = fn_hashcode;
    set->destroy = lock_free_data_free;
    set->remove_from_key = lock_free_data_remove_from_key;
    set->contains_from_key = lock_free_data_contains_from_key;
    set->read = lock_free_data_read;
    set->ls = lock_free_ls;

    // dequeue related
    set->dequeue_item_compare = key_value_database_KV_compare;
    set->dequeue_item_destroy = key_value_database_KV_free;

    set->first_bucket = (Node***) calloc(48, sizeof(Node**));
    check_mem(set->first_bucket, {
        free(set);
        return NULL;
    });

    set->first_bucket[0] = (Node**) calloc(set->size, sizeof(Node*));
    check_mem(set->first_bucket[0], {
        free(set->first_bucket);
        free(set);
        return NULL;
    });

    set->first_bucket[0][0] = (Node*) malloc(1 * sizeof(Node));
    check_mem(set->first_bucket[0][0], {
        free(set->first_bucket[0]);
        free(set->first_bucket);
        free(set);
        return NULL;
    });

    set->first_bucket[0][0]->sentinel = true;
    set->first_bucket[0][0]->next = NULL;
    return set;
}

// TODO do it with allocator node... -> simpler
void lock_free_data_free(Set* self)
{
    for (int i = 0; self->first_bucket[i] != NULL; i++)
    {
        for (int j = 0; self->first_bucket[i][j] != NULL; j++)
        {
            recursive_free(self->first_bucket[i][j]);
        }
        free(self->first_bucket[i]);
    }
    free(self->first_bucket);
    free(self);
}

static void recursive_free(Node* node)
{
    // TODO
    return;
    if (node != NULL)
    {
        recursive_free(node->next);
        free(node);
    }
}

static Node* lock_free_data_initialize_bucket(Set* self, uint32_t key)
{
    Node* sentinel;
    Node* parent_bucket;
    Node* pred;

    uint32_t parent = self->size;
    while ((parent = parent >> 1) > key);
    parent_bucket = lock_free_data_get_secondary_bucket(self, key - parent);

    sentinel = malloc(1 * sizeof(Node));

    // TODO adjust behavior if memory alloc fail ? --> resources to free !
    check_mem_and_exit(sentinel);

    sentinel->sentinel = true;
    sentinel->reversed_key = key = lock_free_data_reverse(key);
    sentinel->next = NULL;

    if (!lock_free_data_list_insert(parent_bucket, sentinel))
    {
        // created by another thread
        free(sentinel);
        if (!lock_free_data_find(parent_bucket, key, true, &pred, &sentinel))
        {
            return NULL;
        }
    }
    return sentinel;
}

static Node* lock_free_data_get_secondary_bucket(Set* self, uint32_t key)
{
    uint32_t i, min, max;
    Node** secondary;
    Node* sentinel;

    for (i = min = 0U, max = 2U; key >= max; i++, max <<= 1)
    {
        min = max;
    }

    if (self->first_bucket[i] == NULL)
    {
        secondary = (Node**) calloc(self->size >> 1, sizeof(Node*));
        check_mem_and_return(secondary, NULL);
        if (!(compare_and_swap(&(self->first_bucket[i]), NULL, secondary)))
            free(secondary);
    }

    if (self->first_bucket[i][key - min] == NULL)
    {
        if ((sentinel = lock_free_data_initialize_bucket(self, key)) != NULL)
        {
            self->first_bucket[i][key - min] = sentinel;
        }
    }

    return self->first_bucket[i][key - min];
}

uint32_t lock_free_data_reverse(uint32_t n)
{
    n = ((n >> 1) & 0x55555555) | ((n << 1) & 0xaaaaaaaa);
    n = ((n >> 2) & 0x33333333) | ((n << 2) & 0xcccccccc);
    n = ((n >> 4) & 0x0f0f0f0f) | ((n << 4) & 0xf0f0f0f0);
    n = ((n >> 8) & 0x00ff00ff) | ((n << 8) & 0xff00ff00);
    n = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000);
    return n;
}

static bool lock_free_data_find(Node* head,
                                uint32_t reversed_key,
                                bool is_sentinel,
                                Node** pred,
                                Node** current)
{
    Conversion next;
    while (true)
    {
        *pred = head;
        *current = (*pred)->next;
        while (true)
        {
            if (*current == NULL)
                return false;
            next.node = (*current)->next;
            if (next.value & 0x1)
            {
                next.value ^= 0x1;
                if (compare_and_swap(&(*pred)->next, *current, next.node))
                    *current = next.node;
                else
                    break;
            }
            else if ((*current)->reversed_key == reversed_key && is_sentinel == (*current)->sentinel)
                return true;
            else if ((*current)->reversed_key == reversed_key && is_sentinel)
                return false; // insert sentinel before
            else if ((*current)->reversed_key > reversed_key)
                return false;
            else
            {
                *pred = *current;
                *current = next.node;
            }
        }
    }
}


bool lock_free_data_add(Set* self, void* data)
{
    Node* bucket, * newNode;
    uint32_t size, key;
    if ((newNode = (Node*) malloc(sizeof(Node))) == NULL)
        return false;
    newNode->sentinel = false;
    key = self->item_hashcode(data);
    newNode->reversed_key = lock_free_data_reverse(key);
    newNode->data = data;
    if ((bucket = lock_free_data_get_secondary_bucket(self, key % self->size)) == NULL)
        return false;
    if (!lock_free_data_list_insert(bucket, newNode))
    {
        free(newNode);
        return false;
    }
    size = self->size;
    if (fetch_and_increment(&(self->item_count)) / size >= MAX_LOAD)
        compare_and_swap(&(self->size), size, size * 2);
    return true;
}


bool lock_free_data_remove(Set* self, void* data)
{
    Node* pred;
    Node* curr;
    Node* bucket;
    Node* tmpNext;
    Conversion next;

    uint32_t key = self->item_hashcode(data);
    if ((bucket = lock_free_data_get_secondary_bucket(self, key % self->size)) == NULL)
        return false;
    key = lock_free_data_reverse(key);
    while (true)
    {
        if (!lock_free_data_find(bucket, key, false, &pred, &curr))
            return false;
        next.node = tmpNext = curr->next;
        if ((next.value & 0x1) == 0)
        {
            next.value |= 0x1;
            if (compare_and_swap(&curr->next, tmpNext, next.node))
            {
                compare_and_swap(&pred->next, curr, tmpNext);
                fetch_and_decrement(&(self->item_count));
                return true;
            }
        }
    }
}


bool lock_free_data_contains(Set* self, void* data)
{
    Node* pred;
    Node* curr;
    Node* bucket;
    Conversion next;

    uint32_t key = self->item_hashcode(data);
    if ((bucket = lock_free_data_get_secondary_bucket(self, key % self->size)) == NULL)
        return false;
    key = lock_free_data_reverse(key);
    while (true)
    {
        if (!lock_free_data_find(bucket, key, false, &pred, &curr))
            return false;
        next.node = curr->next;
        if ((next.value & 0x1) == 0)
        {
            return true;
        }
    }
}

bool lock_free_data_remove_from_key(Set* self, uint32_t key)
{
    Node* pred;
    Node* curr;
    Node* bucket;
    Node* tmpNext;
    Conversion next;

    if ((bucket = lock_free_data_get_secondary_bucket(self, key % self->size)) == NULL)
        return false;
    key = lock_free_data_reverse(key);
    while (true)
    {
        if (!lock_free_data_find(bucket, key, false, &pred, &curr))
            return false;
        next.node = tmpNext = curr->next;
        if ((next.value & 0x1) == 0)
        {
            next.value |= 0x1;
            if (compare_and_swap(&curr->next, tmpNext, next.node))
            {
                compare_and_swap(&pred->next, curr, tmpNext);
                fetch_and_decrement(&(self->item_count));
                return true;
            }
        }
    }
}

bool lock_free_data_contains_from_key(Set* self, uint32_t key)
{
    Node* pred;
    Node* curr;
    Node* bucket;
    Conversion next;

    if ((bucket = lock_free_data_get_secondary_bucket(self, key % self->size)) == NULL)
        return false;
    key = lock_free_data_reverse(key);
    while (true)
    {
        if (!lock_free_data_find(bucket, key, false, &pred, &curr))
            return false;
        next.node = curr->next;
        if ((next.value & 0x1) == 0)
        {
            return true;
        }
    }
}

bool lock_free_data_list_insert(Node* head, Node* node)
{
    Node* p;
    Node* c;
    while (true)
    {
        if (lock_free_data_find(head, node->reversed_key, node->sentinel, &p, &c))
            return false;
        node->next = c;
        if (compare_and_swap(&p->next, c, node))
            return true;
    }
}

bool lock_free_data_add_with_key(Set* self, uint32_t key, void* data)
{
    Node* bucket, * newNode;
    uint32_t size;
    if ((newNode = (Node*) malloc(sizeof(Node))) == NULL)
        return false;
    newNode->sentinel = false;
    newNode->reversed_key = lock_free_data_reverse(key);
    newNode->data = data;
    if ((bucket = lock_free_data_get_secondary_bucket(self, key % self->size)) == NULL)
        return false;
    if (!lock_free_data_list_insert(bucket, newNode))
    {
        free(newNode);
        return false;
    }
    size = self->size;
    if (fetch_and_increment(&(self->item_count)) / size >= MAX_LOAD)
        compare_and_swap(&(self->size), size, size * 2);
    return true;
}

// TODO
void* lock_free_data_read(Set* self, uint32_t key)
{
    Node* pred;
    Node* curr;
    Node* bucket;
    Conversion next;

    if ((bucket = lock_free_data_get_secondary_bucket(self, key % self->size)) == NULL)
    {
        // TODO set ERRNO
        return false;
    }
    key = lock_free_data_reverse(key);
    while (true)
    {
        if (!lock_free_data_find(bucket, key, false, &pred, &curr))
            return false;
        next.node = curr->next;
        if ((next.value & 0x1) == 0)
        {
            return (curr->data);
        }
    }
}

Dequeue* lock_free_ls(Set* set)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(KV), set->dequeue_item_compare, free);
    Node*** firstBucket = set->first_bucket;
    Node** secondBucket;
    Node* node;
    int i = 0, j = 0, k = 0;

    while (*firstBucket != NULL)
    {
        secondBucket = *firstBucket;
        while (*secondBucket != NULL)
        {
            node = *secondBucket;
            while (node != NULL)
            {
                if (!node->sentinel)
                {
                    debug_nl;
                    debug("key : %u\nvalue : %s\n",
                          lock_free_data_reverse(node->reversed_key),
                          *(char**) node->data);
                    debug_nl;
                    Key key = lock_free_data_reverse(node->reversed_key);
                    Value* value = node->data;
                    dequeue->add(dequeue, key_value_database_KV_create(key, *value));
                    debug("Add item on (%d,%d,%d)\n", i, j, k);
                }
                node = node->next;
                k++;
            }
            goto end; // ok...
            j++;
        }
        firstBucket++;
        i++;
    }

    end:
    return dequeue;
}
