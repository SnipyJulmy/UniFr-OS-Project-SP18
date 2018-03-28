//
// Created by snipy on 28.03.18.
//

#include <stdlib.h>

#include "database_actions.h"
#include "lock_free_set/lock_free_set.h"
#include "lock_free_set/lock_free_set_hash.h"

static Set* set;

void database_actions_init(void)
{
    set = lock_free_data_create_set(lock_free_set_hash);
}

void database_actions_free(void)
{
    set->free(set);
}

uint32_t database_actions_insert_v(void* value)
{
    uint32_t key = set->item_hashcode(value);
    if (set->add_with_key(set, key, value))
    {
        return key;
    }
    return NULL;
}

uint32_t database_actions_insert_int(int value)
{
    return 0;
}

bool database_actions_insert_kv(uint32_t key, void* value)
{
    return set->add_with_key(set,key,value);
}

void* database_actions_read_k(uint32_t key)
{
    return NULL;
}

bool database_actions_contains_k(uint32_t key)
{
    return 0;
}

bool database_actions_contains_v(void* value)
{
    return 0;
}

bool database_actions_contains_kv(uint32_t key, void* value)
{
    return 0;
}


