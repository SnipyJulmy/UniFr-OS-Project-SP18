//
// Created by snipy on 04.04.18.
//

#include <stdio.h>
#include "../lib/ctest/ctest.h"
#include "../../src/server/database/database_actions.h"

#define AT(IDX) do {\
    ASSERT_TRUE(database_actions_contains_k(k ## IDX)); \
    ASSERT_TRUE(database_actions_contains_kv(k ## IDX,&s ## IDX)); \
}while(0);

#define AF(IDX) do {\
    ASSERT_FALSE(database_actions_contains_k(k ## IDX)); \
    ASSERT_FALSE(database_actions_contains_kv(k ## IDX,&s ## IDX)); \
}while(0);

CTEST(database_action, insert_v)
{
    database_actions_init();
    char* s1 = "asdisadoan1";
    char* s2 = "asdisadoan2";
    char* s3 = "asdisadoan3";
    char* s4 = "asdisadoan4";
    char* s5 = "asdisadoan5";
    char* s6 = "asdisadoan6";

    Key k1 = database_actions_insert_v(&s1);
    AT(1);
    Key k2 = database_actions_insert_v(&s2);
    AT(1);
    AT(2);
    Key k3 = database_actions_insert_v(&s3);
    AT(1);
    AT(2);
    AT(3);
    Key k4 = database_actions_insert_v(&s4);
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    Key k5 = database_actions_insert_v(&s5);
    AT(1);
    AT(2);
    AT(3);
    AT(4);
    AT(5);
    Key k6 = database_actions_insert_v(&s6);
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
    char* s1 = "asdisadoan1";
    Key k1 = 10;
    char* s2 = "asdisadoan2";
    Key k2 = 11;
    char* s3 = "asdisadoan3";
    Key k3 = 12;
    char* s4 = "asdisadoan4";
    Key k4 = 13;
    char* s5 = "asdisadoan5";
    Key k5 = 14;
    char* s6 = "asdisadoan6";
    Key k6 = 15;

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
    database_actions_destroy();
}

CTEST(database_action, contains_v)
{
    database_actions_init();
    database_actions_destroy();
}

CTEST(database_action, contains_k)
{
    database_actions_init();
    database_actions_destroy();
}

CTEST(database_action, contains_kv)
{
    database_actions_init();
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