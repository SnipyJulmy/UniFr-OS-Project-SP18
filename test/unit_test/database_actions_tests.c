//
// Created by snipy on 04.04.18.
//

#include <stdio.h>
#include <string.h>
#include "../lib/ctest/ctest.h"
#include "../../src/server/database/database_actions.h"
#include "macro_utils.h"

CTEST(database_action, insert_v)
{
    database_actions_init();
    DEF_VAR;
    DEF_KEY;
    k1 = database_actions_insert_v(&s1);
    AT(1);
    k2 = database_actions_insert_v(&s2);
    AT(1);
    AT(2);
    k3 = database_actions_insert_v(&s3);
    AT(1);
    AT(2);
    AT(3);
    k4 = database_actions_insert_v(&s4);
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    k5 = database_actions_insert_v(&s5);
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AT(5);
    k6 = database_actions_insert_v(&s6);
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AT(5);
    AT(6);
    database_actions_destroy();
}

CTEST(database_action, insert_kv)
{
    database_actions_init();
    DEF_VAR;
    DEF_KEY_INIT;

    AF(1);
    AF(2);
    AF(3);
    AF(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(database_actions_insert_kv(k1, &s1));
    AT(1);
    AF(2);
    AF(3);
    AF(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(database_actions_insert_kv(k2, &s2));
    AT(1);
    AT(2);
    AF(3);
    AF(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(database_actions_insert_kv(k3, &s3));
    AT(1);
    AT(2);
    AT(3);
    AF(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(database_actions_insert_kv(k4, &s4));
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AF(5);
    AF(6);
    ASSERT_TRUE(database_actions_insert_kv(k5, &s5));
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AT(5);
    AF(6);
    ASSERT_TRUE(database_actions_insert_kv(k6, &s6));
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AT(5);
    AT(6);

    database_actions_destroy();
}

CTEST(database_action, read_k)
{
    database_actions_init();
    DEF_VAR;
    DEF_KEY;
    ASSERT_TRUE(k1 = database_actions_insert_v(&s1));
    ASSERT_TRUE(k2 = database_actions_insert_v(&s2));
    ASSERT_TRUE(k3 = database_actions_insert_v(&s3));
    ASSERT_TRUE(k4 = database_actions_insert_v(&s4));
    ASSERT_TRUE(k5 = database_actions_insert_v(&s5));
    ASSERT_TRUE(k6 = database_actions_insert_v(&s6));
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AT(5);
    AT(6);

    ASSERT_TRUE(database_actions_read_v_from_key(k1) == &s1);
    ASSERT_TRUE(database_actions_read_v_from_key(k2) == &s2);
    ASSERT_TRUE(database_actions_read_v_from_key(k3) == &s3);
    ASSERT_TRUE(database_actions_read_v_from_key(k4) == &s4);
    ASSERT_TRUE(database_actions_read_v_from_key(k5) == &s5);
    ASSERT_TRUE(database_actions_read_v_from_key(k6) == &s6);
    ASSERT_TRUE(strcmp(*database_actions_read_v_from_key(k1), *database_actions_read_v_from_key(k1)) == 0);
    ASSERT_TRUE(strcmp(*database_actions_read_v_from_key(k2), *database_actions_read_v_from_key(k2)) == 0);
    ASSERT_TRUE(strcmp(*database_actions_read_v_from_key(k3), *database_actions_read_v_from_key(k3)) == 0);
    ASSERT_TRUE(strcmp(*database_actions_read_v_from_key(k4), *database_actions_read_v_from_key(k4)) == 0);
    ASSERT_TRUE(strcmp(*database_actions_read_v_from_key(k5), *database_actions_read_v_from_key(k5)) == 0);
    ASSERT_TRUE(strcmp(*database_actions_read_v_from_key(k6), *database_actions_read_v_from_key(k6)) == 0);
    ASSERT_TRUE(strcmp(*database_actions_read_v_from_key(k1), *database_actions_read_v_from_key(k4)) == 0);
    ASSERT_TRUE(strcmp(*database_actions_read_v_from_key(k2), *database_actions_read_v_from_key(k5)) == 0);
    ASSERT_TRUE(strcmp(*database_actions_read_v_from_key(k3), *database_actions_read_v_from_key(k6)) == 0);

    database_actions_destroy();
}

CTEST(database_action, contains_k)
{
    database_actions_init();

    DEF_VAR;
    DEF_KEY_INIT;
    INSERT_ALL;

    CONTAINS_ALL_KEY;
    ASSERT_TRUE(database_actions_remove_from_k(k1));
    CONTAINS_KEYS_BUT(1);
    ASSERT_TRUE(database_actions_insert_kv(k1, &s1));
    CONTAINS_ALL_KEY;
    ASSERT_TRUE(database_actions_remove_from_k(k1));
    ASSERT_TRUE(database_actions_remove_from_k(k2));
    CONTAINS_KEY_4(3, 4, 5, 6);
    CONTAINS_NOT_KEY_2(1, 2);
    ASSERT_TRUE(database_actions_insert_kv(k1, &s1));
    ASSERT_TRUE(database_actions_insert_kv(k2, &s2));
    CONTAINS_ALL_KEY;
    REMOVE(1);
    CONTAINS_KEY_5(2, 3, 4, 5, 6);
    CONTAINS_NOT_KEY_1(1);
    REMOVE(2);
    CONTAINS_KEY_4(3, 4, 5, 6);
    CONTAINS_NOT_KEY_2(1, 2);
    REMOVE(3);
    CONTAINS_KEY_3(4, 5, 6);
    CONTAINS_NOT_KEY_3(1, 2, 3);
    REMOVE(4);
    CONTAINS_KEY_2(5, 6);
    CONTAINS_NOT_KEY_4(1, 2, 3, 4);
    REMOVE(5);
    CONTAINS_KEY_1(6);
    CONTAINS_NOT_KEY_5(1, 2, 3, 4, 5);
    REMOVE(6);
    CONTAINS_NOT_KEY_6(1, 2, 3, 4, 5, 6);

    database_actions_destroy();
}

CTEST(database_action, contains_kv)
{
    database_actions_init();
    DEF_VAR;
    DEF_KEY_INIT;
    INSERT_ALL;

    CONTAINS_ALL_KEY_VALUE;
    REMOVE(1);

    CONTAINS_NOT_KEY_VALUE_1(1);
    CONTAINS_KEY_VALUE_5(2, 3, 4, 5, 6);
    INSERT_KV(1);
    CONTAINS_ALL_KEY_VALUE;
    REMOVE(1);
    REMOVE(2);
    CONTAINS_KEY_VALUE_4(3, 4, 5, 6);
    CONTAINS_NOT_KEY_VALUE_2(1, 2);
    INSERT_KV(1);
    INSERT_KV(2);
    CONTAINS_ALL_KEY_VALUE;
    REMOVE(1);
    CONTAINS_KEY_VALUE_5(2, 3, 4, 5, 6);
    CONTAINS_NOT_KEY_VALUE_1(1);
    REMOVE(2);
    CONTAINS_KEY_VALUE_4(3, 4, 5, 6);
    CONTAINS_NOT_KEY_VALUE_2(1, 2);
    REMOVE(3);
    CONTAINS_KEY_VALUE_3(4, 5, 6);
    CONTAINS_NOT_KEY_VALUE_3(1, 2, 3);
    REMOVE(4);
    CONTAINS_KEY_VALUE_2(5, 6);
    CONTAINS_NOT_KEY_VALUE_4(1, 2, 3, 4);
    REMOVE(5);
    CONTAINS_KEY_VALUE_1(6);
    CONTAINS_NOT_KEY_VALUE_5(1, 2, 3, 4, 5);
    REMOVE(6);
    CONTAINS_NOT_KEY_VALUE_6(1, 2, 3, 4, 5, 6);

    database_actions_destroy();
}

CTEST(database_actions, remove_k)
{
    database_actions_init();
    database_actions_destroy();
}

CTEST(database_actions, remove_kv)
{
    database_actions_init();
    database_actions_destroy();
}