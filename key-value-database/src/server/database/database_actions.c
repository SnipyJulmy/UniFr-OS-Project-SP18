//
// Created by snipy on 28.03.18.
//

#include <stdlib.h>

#include "database_actions.h"
#include "lock_free_set/lock_free_set.h"
#include "lock_free_set/lock_free_set_hash.h"
#include "../../debug.h"

static Set* set;

// init the database
void database_actions_init(void)
{
    set = lock_free_data_create_set(lock_free_set_hash);
    check_mem_and_exit(set);
}

// free all the ressources used by the database
void database_actions_destroy(void)
{
    set->free(set);
}

Key database_actions_insert_v(Value* value)
{
    Key key = set->item_hashcode(value);
    if (set->add_with_key(set, key, value))
    {
        return key;
    }
    return NULL;
}

bool database_actions_insert_kv(Key key, Value* value)
{
    return set->add_with_key(set, key, value);
}

Value* database_actions_read_k(Key key)
{
    return set->read(set, key);
}

bool database_actions_contains_k(Key key)
{
    return set->contains_from_key(set, key);
}

bool database_actions_contains_kv(Key key, Value* value)
{
    Value* res = set->read(set, key);
    return res == value;
}

bool database_actions_remove_k(Key key)
{
    return set->remove_from_key(set,key);
}

bool database_actions_remove_kv(Key key, Value* value)
{
    Value* res = set->read(set,key);
    if(value == res)
        return set->remove_from_key(set,key);
    return false;
}
