//
// Created by snipy on 02.04.18.
//

#ifndef KEY_VALUE_DATABASE_KEY_VALUE_DATABASE_TYPEDEF_H
#define KEY_VALUE_DATABASE_KEY_VALUE_DATABASE_TYPEDEF_H

#include <stdint.h>
#include <stdlib.h>

typedef uint32_t Key;
typedef char* Value;
typedef struct
{
    Key key;
    Value value;
} KV;

int key_value_database_KV_compare(void* a, void* b);

void key_value_database_KV_free(void* kv);

#endif //KEY_VALUE_DATABASE_KEY_VALUE_DATABASE_TYPEDEF_H
