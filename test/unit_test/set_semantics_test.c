//
// Created by snipy on 15.03.18.
//

#include <stdio.h>
#include <stdlib.h>

#include "../lib/ctest/ctest.h"
#include "../../src/data_structures/parallel/lock_free_set/lock_free_set.h"
#include "../../src/data_structures/parallel/lock_free_set/lock_free_set_hash.h"
#include "macro_utils.h"

#define FREE_TEST_VECTOR_LENGTH 5

#define ADD_GEN(K, V, T) do{T value = V; set->add_with_key(set,K,&value);}while(0);
#define ADD(K, V) ADD_GEN(K,V,int)

CTEST(set_semantics_test, simple_int)
{
    char* a = "a";
    char* b = "b";
    char* c = "c";

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
    set->destroy(set);
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

    set->destroy(set);
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
    set->destroy(set);
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

    set->destroy(set);

    // free resources
    free(vector);
}

CTEST(set_semantics_test, contains_value)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);

    char* s1 = "asd1";
    char* s2 = "asd2";
    char* s3 = "asd3";
    char* s4 = "asd4";
    char* s5 = "asd5";
    char* s6 = "asd6";

    AF_SET(1);
    AF_SET(2);
    AF_SET(3);
    AF_SET(4);
    AF_SET(5);
    AF_SET(6);
    ASSERT_TRUE(set->add(set, &s1));
    AT_SET(1);
    AF_SET(2);
    AF_SET(3);
    AF_SET(4);
    AF_SET(5);
    AF_SET(6);
    ASSERT_TRUE(set->add(set, &s2));
    AT_SET(1);
    AT_SET(2);
    AF_SET(3);
    AF_SET(4);
    AF_SET(5);
    AF_SET(6);
    ASSERT_TRUE(set->add(set, &s3));
    AT_SET(1);
    AT_SET(2);
    AT_SET(3);
    AF_SET(4);
    AF_SET(5);
    AF_SET(6);
    ASSERT_TRUE(set->add(set, &s4));
    AT_SET(1);
    AT_SET(2);
    AT_SET(3);
    AT_SET(4);
    AF_SET(5);
    AF_SET(6);
    ASSERT_TRUE(set->add(set, &s5));
    AT_SET(1);
    AT_SET(2);
    AT_SET(3);
    AT_SET(4);
    AT_SET(5);
    AF_SET(6);
    ASSERT_TRUE(set->add(set, &s6));
    AT_SET(1);
    AT_SET(2);
    AT_SET(3);
    AT_SET(4);
    AT_SET(5);
    AT_SET(6);

    ASSERT_TRUE(set->contains_from_key(set, set->item_hashcode(&s1)));

    set->destroy(set);
}

CTEST(set_semantics_test, ls)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);

    DEF_VAR;
    DEF_KEY_INIT;

    ASSERT_TRUE(set->add_with_key(set, k1, &s1));
    ASSERT_TRUE(set->add_with_key(set, k2, &s2));
    ASSERT_TRUE(set->add_with_key(set, k3, &s3));
    ASSERT_TRUE(set->add_with_key(set, k4, &s4));
    ASSERT_TRUE(set->add_with_key(set, k5, &s5));
    ASSERT_TRUE(set->add_with_key(set, k6, &s6));
    ASSERT_TRUE(set->add_with_key(set, k7, &s7));
    ASSERT_TRUE(set->add_with_key(set, k8, &s8));
    ASSERT_TRUE(set->add_with_key(set, k9, &s9));
    ASSERT_TRUE(set->add_with_key(set, k10, &s10));
    ASSERT_TRUE(set->add_with_key(set, k11, &s11));
    ASSERT_TRUE(set->add_with_key(set, k12, &s12));
    ASSERT_TRUE(set->add_with_key(set, k13, &s13));
    ASSERT_TRUE(set->add_with_key(set, k14, &s14));
    ASSERT_TRUE(set->add_with_key(set, k15, &s15));
    ASSERT_TRUE(set->add_with_key(set, k16, &s16));
    ASSERT_TRUE(set->add_with_key(set, k17, &s17));

    Dequeue* dequeue = set->ls(set);

    ASSERT_TRUE(dequeue->size == 17);

    dequeue->destroy(dequeue);
    set->destroy(set);
}

CTEST(set_semantics_test, update)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);
    DEF_VAR;
    DEF_KEY_INIT;
    ASSERT_TRUE(set->add_with_key(set, k1, &s1));
    ASSERT_TRUE(set->update(set, k1, &s2));

    Dequeue* dequeue = set->ls(set);
    ASSERT_TRUE(dequeue->size == 1);

    set->destroy(set);
}

CTEST(set_semantics_test, remove_string_value)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);

    char* a = "asd1";
    Key keyA = set->item_hashcode(&a);
    char* b = "asd2";
    Key keyB = set->item_hashcode(&b);
    char* c = "asd1";
    Key keyC = set->item_hashcode(&c);
    char* d = "asd2";
    Key keyD = set->item_hashcode(&d);

    ASSERT_TRUE(set->add_with_key(set, keyA, &a));
    ASSERT_TRUE(set->add_with_key(set, keyB, &b));
    ASSERT_TRUE(set->contains_from_key(set, keyA));
    ASSERT_TRUE(set->contains_from_key(set, keyB));
    ASSERT_TRUE(set->contains(set, &a));
    ASSERT_TRUE(set->contains(set, &b));
    ASSERT_FALSE(set->contains_from_key(set, keyC));
    ASSERT_FALSE(set->contains_from_key(set, keyD));
    ASSERT_TRUE(set->contains(set, &c));
    ASSERT_TRUE(set->contains(set, &d));
    ASSERT_TRUE(set->remove_from_key(set,keyA));
    ASSERT_FALSE(set->contains(set, &a));
    ASSERT_FALSE(set->contains(set, &c));
    ASSERT_FALSE(set->contains_from_key(set, keyA));
    ASSERT_FALSE(set->contains_from_key(set, keyC));
    ASSERT_FALSE(set->contains_from_key(set, keyD));
    ASSERT_TRUE(set->remove_from_value(set,&d));
    ASSERT_FALSE(set->contains_from_key(set, keyA));
    ASSERT_FALSE(set->contains_from_key(set, keyB));
    ASSERT_FALSE(set->contains_from_key(set, keyC));
    ASSERT_FALSE(set->contains_from_key(set, keyD));

    ASSERT_TRUE(set->item_count == 0);

    ASSERT_TRUE(set->add_with_key(set, keyA, &a));
    ASSERT_TRUE(set->add_with_key(set, keyB, &b));
    ASSERT_TRUE(set->contains_from_key(set, keyA));
    ASSERT_TRUE(set->contains_from_key(set, keyB));
    ASSERT_TRUE(set->remove_from_value(set,&c));
    ASSERT_TRUE(set->remove_from_value(set,&d));
    ASSERT_FALSE(set->contains_from_key(set, keyA));
    ASSERT_FALSE(set->contains_from_key(set, keyB));
    ASSERT_FALSE(set->contains_from_key(set, keyC));
    ASSERT_FALSE(set->contains_from_key(set, keyD));
    ASSERT_TRUE(set->add_with_key(set, keyA, &a));
    ASSERT_TRUE(set->add_with_key(set, keyB, &b));
    ASSERT_TRUE(set->contains_from_key(set, keyA));
    ASSERT_TRUE(set->contains_from_key(set, keyB));
    ASSERT_TRUE(set->remove_from_value(set,&a));
    ASSERT_TRUE(set->remove_from_value(set,&b));
    ASSERT_FALSE(set->contains_from_key(set, keyA));
    ASSERT_FALSE(set->contains_from_key(set, keyB));
    ASSERT_FALSE(set->contains_from_key(set, keyC));
    ASSERT_FALSE(set->contains_from_key(set, keyD));

    set->destroy(set);
}
