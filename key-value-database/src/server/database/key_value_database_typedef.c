//
// Created by snipy on 18.04.18.
//

#include "key_value_database_typedef.h"

int key_value_database_KV_compare(void* a, void* b)
{
    KV* pA = a;
    KV* pB = b;
    return pA->key - pB->key;
}

void key_value_database_KV_free(void* kv)
{
    free(kv);
}

