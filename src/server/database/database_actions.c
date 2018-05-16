//
// Created by snipy on 28.03.18.
//

#include <stdlib.h>
#include <assert.h>

#include "database_actions.h"
#include "../../utils/debug.h"
#include "../../data_structures/parallel/lock_free_set/lock_free_set.h"
#include "../../data_structures/parallel/lock_free_set/lock_free_set_hash.h"

static Set* set;

// init the database
void database_actions_init(void)
{
    set = lock_free_data_create_set(lock_free_set_hash);
    check_mem_and_exit(set);
}

// free all the resources used by the database
void database_actions_destroy(void)
{
    set->destroy(set);
}

Key database_actions_insert_v(Value* value)
{
    Key key = set->item_hashcode(value);
    if (set->add_with_key(set, key, value))
    {
        assert(key != 0);
        return key;
    }
    return 0;
}

bool database_actions_insert_kv(Key key, Value* value)
{
    return set->add_with_key(set, key, value);
}

Value* database_actions_read_v_from_key(Key key)
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
    if (res == NULL) return false;
    return strcmp(*value, *res) == 0;
}

bool database_actions_remove_from_k(Key key)
{
    return set->remove_from_key(set, key);
}

bool database_actions_remove_from_kv(Key key, Value* value)
{
    Value* res = set->read(set, key);
    if (value == res)
        return set->remove_from_key(set, key);
    return false;
}

Dequeue* database_actions_ls(void)
{
    return set->ls(set);
}

bool database_actions_update_kv(Key key, Value* value)
{
    return set->update(set, key, value);
}

bool database_actions_contains_v(Value* value)
{
    return set->contains_from_value(set, value);
}

Key database_actions_read_k_from_v(Value* value)
{
    return set->read_key_from_value(set, value);
}

bool database_actions_remove_from_v(Value* value)
{
    return set->remove_from_value(set, value);
}
