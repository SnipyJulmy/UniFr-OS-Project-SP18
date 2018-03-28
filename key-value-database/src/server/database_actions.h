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

uint32_t database_actions_insert_v(void* value);
uint32_t database_actions_insert_int(int value);
bool database_actions_insert_kv(uint32_t key, void* value);
void* database_actions_read_k(uint32_t key);
bool database_actions_contains_k(uint32_t key);
bool database_actions_contains_v(void* value);
bool database_actions_contains_kv(uint32_t key, void* value);
void database_actions_init(void);

#endif //KEY_VALUE_DATABASE_DATABASE_ACTIONS_H
