//
// Created by snipy on 16.03.18.
//
#include <stdio.h>
#include "../lib/ctest/ctest.h"
#include "../../src/server/database/lock_free_set/lock_free_set.h"

CTEST(reverse, reverse_key)
{
    // Test some number with the identity function reverse_key(reverse_key(key)) == key
    for (uint32_t i = 0; i < (UINT32_MAX / 2); i += (UINT32_MAX / 100))
    {
        ASSERT_TRUE(lock_free_data_reverse(lock_free_data_reverse(i)) == i);
    }
}

