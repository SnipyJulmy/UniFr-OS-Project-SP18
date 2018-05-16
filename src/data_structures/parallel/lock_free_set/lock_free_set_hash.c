//
// Created by snipy on 16.03.18.
//

#include "lock_free_set_hash.h"

uint32_t lock_free_set_hash(void* s)
{
    // obviously the most thoughtful implementation
    // of an hash function
    return (uint32_t) s;
}