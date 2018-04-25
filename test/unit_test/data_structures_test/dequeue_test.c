//
// Created by snipy on 18.04.18.
//

#include "../../lib/ctest/ctest.h"
#include "../../../src/data_structures/dequeue/dequeue.h"
#include "../../../src/debug.h"
#include "dequeue_test_macro.h"


int int_cmp(void* a, void* b)
{
    int* pA = a;
    int* pB = b;
    return *pA - *pB;
}

void int_destroy(void* pInt)
{
    // nothing...
}

CTEST(dequeue, simple_int_usage)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(int), int_cmp, int_destroy);

    VALUE(1);
    VALUE(2);
    VALUE(3);
    VALUE(4);

    ASSERT_FALSE(dequeue->contains(dequeue, p1));
    ASSERT_FALSE(dequeue->contains(dequeue, p2));
    ASSERT_FALSE(dequeue->contains(dequeue, p3));
    ASSERT_FALSE(dequeue->contains(dequeue, p4));

    dequeue->destroy(dequeue);
}

CTEST(dequeue, add)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(int), int_cmp, int_destroy);
    check_mem_and_exit(dequeue);

    VALUE(1);
    VALUE(2);
    VALUE(3);
    VALUE(4);
    VALUE(5);
    VALUE(6);

    ASSERT_TRUE(dequeue->is_empty(dequeue));
    ASSERT_TRUE(dequeue->size == 0);

    ADD(1);
    AT_FIRST(1);
    ASSERT_SIZE(1);

    ADD(2);
    AT_FIRST(1);
    ASSERT_SIZE(2);

    ADD(3);
    AT_FIRST(1);
    ASSERT_SIZE(3);

    ADD(4);
    AT_FIRST(1);
    ASSERT_SIZE(4);

    ADD(5);
    AT_FIRST(1);
    ASSERT_SIZE(5);

    ADD_FIRST(6);
    AT_FIRST(6);
    ASSERT_SIZE(6);

    dequeue->destroy(dequeue);
}

CTEST(dequeue, remove)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(int), int_cmp, int_destroy);
    check_mem_and_exit(dequeue);
    VALUES_1_6;

    ADD(1);
    AT_FIRST(1);
    ASSERT_SIZE(1);
    REMOVE(1);
    ASSERT_SIZE(0);
    ADD(2);
    AT_FIRST(2);
    ASSERT_SIZE(1);
    ADD(3);
    AT_FIRST(2);
    ASSERT_SIZE(2);
    ADD_FIRST(4);
    AT_FIRST(4);
    ASSERT_SIZE(3);
    ADD_FIRST(5);
    AT_FIRST(5);
    ASSERT_SIZE(4);
    ADD_FIRST(6);
    AT_FIRST(6);
    ASSERT_SIZE(5);
    ADD(1);
    AT_FIRST(6);
    ASSERT_SIZE(6);
    REMOVE(1);
    AT_FIRST(6);
    ASSERT_SIZE(5)
    REMOVE(2);
    AT_FIRST(6);
    ASSERT_SIZE(4);
    REMOVE(3);
    AT_FIRST(6);
    ASSERT_SIZE(3);
    REMOVE(4);
    AT_FIRST(6);
    ASSERT_SIZE(2);
    REMOVE(5);
    AT_FIRST(6);
    ASSERT_SIZE(1);
    REMOVE(6);
    ASSERT_SIZE(0);
    ADD_ALL;
    ASSERT_SIZE(6);
    ASSERT_TRUE(REMOVE_FIRST_V == 6);
    ASSERT_SIZE(5);
    ASSERT_TRUE(REMOVE_FIRST_V == 5);
    ASSERT_SIZE(4);
    ASSERT_TRUE(REMOVE_FIRST_V == 4);
    ASSERT_SIZE(3);
    ASSERT_TRUE(REMOVE_FIRST_V == 3);
    ASSERT_SIZE(2);
    ASSERT_TRUE(REMOVE_FIRST_V == 2);
    ASSERT_SIZE(1);
    ASSERT_TRUE(REMOVE_FIRST_V == 1);
    ASSERT_SIZE(0);
    ADD_ALL;
    ASSERT_SIZE(6);
    ASSERT_TRUE(REMOVE_LAST_V == 1);
    ASSERT_SIZE(5);
    ASSERT_TRUE(REMOVE_LAST_V == 2);
    ASSERT_SIZE(4);
    ASSERT_TRUE(REMOVE_LAST_V == 3);
    ASSERT_SIZE(3);
    ASSERT_TRUE(REMOVE_LAST_V == 4);
    ASSERT_SIZE(2);
    ASSERT_TRUE(REMOVE_LAST_V == 5);
    ASSERT_SIZE(1);
    ASSERT_TRUE(REMOVE_LAST_V == 6);
    ASSERT_SIZE(0);

    dequeue->destroy(dequeue);
}

CTEST(dequeue, contains)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(int), int_cmp, int_destroy);
    check_mem_and_exit(dequeue);
    dequeue->destroy(dequeue);
}

CTEST(dequeue, move_n_first_to_front)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(int), int_cmp, int_destroy);
    check_mem_and_exit(dequeue);
    dequeue->destroy(dequeue);
}

CTEST(dequeue, move_n_last_to_front)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(int), int_cmp, int_destroy);
    check_mem_and_exit(dequeue);
    dequeue->destroy(dequeue);
}

CTEST(dequeue, move_n_first_to_end)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(int), int_cmp, int_destroy);
    check_mem_and_exit(dequeue);
    dequeue->destroy(dequeue);
}

CTEST(dequeue, move_n_last_to_end)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(int), int_cmp, int_destroy);
    check_mem_and_exit(dequeue);
    dequeue->destroy(dequeue);
}
