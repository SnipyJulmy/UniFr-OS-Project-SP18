//
// Created by snipy on 15.03.18.
//

#include <stdio.h>
#include <stdlib.h>

#include "../lib/ctest/ctest.h"
#include "../../src/server/database/lock_free_set/lock_free_set.h"
#include "../../src/server/database/lock_free_set/lock_free_set_hash.h"

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

CTEST(set_semantics_test, set_operation_read)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);

    #pragma omp parallel default(none) num_threads(8) shared(set)
    {
        char* s1 = "asd";
        Key key1 = set->item_hashcode(&s1);
        ASSERT_TRUE(set->add_with_key(set, key1, &s1));
        ASSERT_TRUE(set->read(set, key1) == &s1);
        ASSERT_TRUE(set->remove_from_key(set, key1));
        ASSERT_TRUE(set->add_with_key(set, key1, &s1));
        ASSERT_TRUE(set->read(set, key1) == &s1);
        ASSERT_TRUE(set->remove_from_key(set, key1));
        ASSERT_TRUE(set->add_with_key(set, key1, &s1));
        ASSERT_TRUE(set->read(set, key1) == &s1);
        ASSERT_TRUE(set->remove_from_key(set, key1));

        char* s2 = "asd2";
        Key key2 = set->item_hashcode(&s2);
        char* s3 = "asd3";
        Key key3 = set->item_hashcode(&s3);
        char* s4 = "asd4";
        Key key4 = set->item_hashcode(&s4);

        ASSERT_TRUE(set->add_with_key(set, key2, &s2));
        ASSERT_TRUE(set->add_with_key(set, key3, &s3));
        ASSERT_TRUE(set->add_with_key(set, key4, &s4));
        ASSERT_TRUE(set->read(set, key2) == &s2);
        ASSERT_TRUE(set->read(set, key3) == &s3);
        ASSERT_TRUE(set->read(set, key4) == &s4);
        ASSERT_FALSE(set->remove_from_key(set, key1));
        ASSERT_TRUE(set->read(set, key2) == &s2);
        ASSERT_TRUE(set->read(set, key3) == &s3);
        ASSERT_TRUE(set->read(set, key4) == &s4);
        ASSERT_TRUE(set->add_with_key(set, key1, &s1));
        ASSERT_TRUE(set->read(set, key2) == &s2);
        ASSERT_TRUE(set->read(set, key3) == &s3);
        ASSERT_TRUE(set->read(set, key4) == &s4);
        ASSERT_TRUE(set->remove_from_key(set, key1));
        ASSERT_TRUE(set->read(set, key2) == &s2);
        ASSERT_TRUE(set->read(set, key3) == &s3);
        ASSERT_TRUE(set->read(set, key4) == &s4);
        ASSERT_TRUE(set->remove_from_key(set, key2));
        ASSERT_TRUE(set->read(set, key3) == &s3);
        ASSERT_TRUE(set->read(set, key4) == &s4);
        ASSERT_TRUE(set->remove_from_key(set, key3));
        ASSERT_TRUE(set->read(set, key4) == &s4);
        ASSERT_TRUE(set->remove_from_key(set, key4));
        ASSERT_FALSE(set->read(set, key1) == &s1);
        ASSERT_FALSE(set->read(set, key2) == &s2);
        ASSERT_FALSE(set->read(set, key3) == &s3);
        ASSERT_FALSE(set->read(set, key4) == &s4);
    }
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

#define AF(IDX) do{\
    ASSERT_FALSE(set->contains(set,&s ## IDX)); \
    ASSERT_FALSE(set->contains_from_key(set, set->item_hashcode(&s ## IDX))); \
    }while(0);
#define AT(IDX) do{\
    ASSERT_TRUE(set->contains(set,&s ## IDX)); \
    ASSERT_TRUE(set->contains_from_key(set, set->item_hashcode(&s ## IDX))); \
    }while(0);
CTEST(set_semantics_test, contains_value)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);

    char* s1 = "asd1";
    char* s2 = "asd2";
    char* s3 = "asd2";
    char* s4 = "asd2";
    char* s5 = "asd2";
    char* s6 = "asd2";

    AF(1);
    AF(2);
    AF(3);
    AF(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(set->add(set, &s1));
    AT(1);
    AF(2);
    AF(3);
    AF(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(set->add(set, &s2));
    AT(1);
    AT(2);
    AF(3);
    AF(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(set->add(set, &s3));
    AT(1);
    AT(2);
    AT(3);
    AF(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(set->add(set, &s4));
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(set->add(set, &s5));
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AT(5);
    AF(6);
    ASSERT_TRUE(set->add(set, &s6));
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AT(5);
    AT(6);

    ASSERT_TRUE(set->contains_from_key(set, set->item_hashcode(&s1)));

    set->free(set);
}