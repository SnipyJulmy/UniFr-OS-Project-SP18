//
// Created by snipy on 28.03.18.
//

/*
 *  List of available command to send to the server
 *  "$>" indicates the terminal like interface
 *  ">>>" indicates the value return by the server
 *
 *  $> insert <value> >>> <key>
 *  $> insert <key> <value> >>> <bool>
 *  $> read <key> >>> <value>
 *  $> contains <key> >>> <bool>
 *  $> contains <value> >>> <bool>
 *  $> contains <key> <value> >>> <bool>
 */

#ifndef KEY_VALUE_DATABASE_DATABASE_ACTIONS_H
#define KEY_VALUE_DATABASE_DATABASE_ACTIONS_H

#include <stdint.h>
#include <stdbool.h>
#include "key_value_database_typedef.h"
#include "../../data_structures/dequeue/dequeue.h"

void database_actions_init(void);
void database_actions_destroy(void);

Key database_actions_insert_v(Value* value);
bool database_actions_insert_kv(Key key, Value* value);
Value* database_actions_read_v_from_key(Key key);
Key database_actions_read_k_from_v(Value* value);
bool database_actions_contains_k(Key key);
bool database_actions_contains_kv(Key key, Value* value);
bool database_actions_contains_v(Value* value);
bool database_actions_remove_from_k(Key key);
bool database_actions_remove_from_kv(Key key, Value* value);
bool database_actions_remove_from_v(Value* value);
bool database_actions_update_kv(Key key, Value* value);
Dequeue* database_actions_ls(void);

#endif //KEY_VALUE_DATABASE_DATABASE_ACTIONS_H
