//
// Created by snipy on 15.03.18.
//

#include <stdio.h>
#include <stdlib.h>

#include "../lib/ctest/ctest.h"
#include "../../src/server/lock_free_set/lock_free_set.h"
#include "../../src/server/lock_free_set/lock_free_set_hash.h"

#define FREE_TEST_VECTOR_LENGTH 5

#define ADD_GEN(K, V, T) do{T value = V; set->add_with_key(set,K,&value);}while(0);
#define ADD(K, V) ADD_GEN(K,V,int)

CTEST(set_semantics_test, simple_int)
{
    int a = 8;
    int b = 9;
    int c = 10;

    Set* set = lock_free_data_create_set(lock_free_set_hash);
    ASSERT_NOT_NULL(set);

    // Simple insert and remove with simple type
    ASSERT_TRUE(set->item_count == 0);
    ASSERT_TRUE(set->add(set, &a));
    ASSERT_TRUE(set->item_count == 1);
    ASSERT_TRUE(set->remove(set, &a));
    ASSERT_FALSE(set->contains(set, &a));
    ASSERT_FALSE(set->contains(set, &b));
    ASSERT_FALSE(set->contains(set, &c));
    ASSERT_TRUE(set->add(set, &a));
    ASSERT_TRUE(set->contains(set, &a));
    ASSERT_TRUE(set->contains(set, &a));
    ASSERT_TRUE(set->contains(set, &a));
    ASSERT_TRUE(set->item_count == 1);
    ASSERT_TRUE(set->add(set, &b));
    ASSERT_TRUE(set->contains(set, &b));
    ASSERT_TRUE(set->contains(set, &a));
    ASSERT_TRUE(set->item_count == 2);
    ASSERT_TRUE(set->add(set, &c));
    ASSERT_TRUE(set->contains(set, &a));
    ASSERT_TRUE(set->contains(set, &b));
    ASSERT_TRUE(set->contains(set, &c));
    ASSERT_TRUE(set->item_count == 3);
    ASSERT_TRUE(set->remove(set, &a));
    ASSERT_TRUE(set->item_count == 2);
    ASSERT_TRUE(set->remove(set, &b));
    ASSERT_TRUE(set->item_count == 1);
    ASSERT_TRUE(set->remove(set, &c));
    ASSERT_TRUE(set->item_count == 0);
    set->free(set);
}

CTEST(set_semantics_test, user_key)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);

    int a = 2;
    ASSERT_TRUE(set->add_with_key(set, 10, &a));
    ASSERT_TRUE(set->contains_from_key(set, 10));
    ASSERT_TRUE(set->remove_from_key(set, 10));

    ASSERT_TRUE(set->add_with_key(set, 11, &a));
    ASSERT_TRUE(set->contains_from_key(set, 11));
    ASSERT_TRUE(set->remove_from_key(set, 11));

    ASSERT_TRUE(set->add_with_key(set, 12, &a));
    ASSERT_TRUE(set->contains_from_key(set, 12));
    ASSERT_TRUE(set->remove_from_key(set, 12));

    set->free(set);
}

CTEST(set_semantics_test, set_operation_with_int)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);

    int a = 2;
    ASSERT_TRUE(set->add_with_key(set,10,&a));
    int b = set->read_int(set,10);
    ASSERT_TRUE(a == b);
    set->free(set);
}

CTEST(set_semantics_test, free)
{
    // Init a range of int
    int* vector = malloc(FREE_TEST_VECTOR_LENGTH * sizeof(int));

    Set* set = lock_free_data_create_set(lock_free_set_hash);

    for (int i = 0; i < FREE_TEST_VECTOR_LENGTH; ++i)
    {
        ASSERT_TRUE(set->add(set, &(vector[i])));
    }

    set->free(set);

    // free resources
    free(vector);
}

