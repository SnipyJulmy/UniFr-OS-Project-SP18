//
// Created by snipy on 05.04.18.
//

#ifndef KEY_VALUE_DATABASE_MACRO_UTILS_H
#define KEY_VALUE_DATABASE_MACRO_UTILS_H

#define PROTECT(STATEMENTS) do {STATEMENTS;} while(0);

#define AT(IDX) do {\
    ASSERT_TRUE(database_actions_contains_k(k ## IDX)); \
    ASSERT_TRUE(database_actions_contains_kv(k ## IDX,&s ## IDX)); \
}while(0);

#define AF(IDX) do {\
    ASSERT_FALSE(database_actions_contains_k(k ## IDX)); \
    ASSERT_FALSE(database_actions_contains_kv(k ## IDX,&s ## IDX)); \
}while(0);

#define DEF_VAR \
    char* s1 = "asdisadoan1";\
    char* s2 = "asdisadoan2";\
    char* s3 = "asdisadoan3";\
    char* s4 = "asdisadoan1";\
    char* s5 = "asdisadoan2";\
    char* s6 = "asdisadoan3";

#define DEF_KEY \
    Key k1; \
    Key k2; \
    Key k3; \
    Key k4; \
    Key k5; \
    Key k6;

#define DEF_KEY_INIT \
    Key k1 = 10; \
    Key k2 = 11; \
    Key k3 = 12; \
    Key k4 = 13; \
    Key k5 = 14; \
    Key k6 = 15;

#define INSERT_ALL \
    ASSERT_TRUE(k1 = database_actions_insert_v(&s1)); \
    ASSERT_TRUE(k2 = database_actions_insert_v(&s2)); \
    ASSERT_TRUE(k3 = database_actions_insert_v(&s3)); \
    ASSERT_TRUE(k4 = database_actions_insert_v(&s4)); \
    ASSERT_TRUE(k5 = database_actions_insert_v(&s5)); \
    ASSERT_TRUE(k6 = database_actions_insert_v(&s6));

#define CONTAINS_ALL_KEY CONTAINS_KEY_6(1,2,3,4,5,6);

#define CONTAINS_KEYS_BUT(N) \
if ((N)==1) do {\
    ASSERT_FALSE(database_actions_contains_k(k1)); \
    ASSERT_TRUE(database_actions_contains_k(k2));  \
    ASSERT_TRUE(database_actions_contains_k(k3));  \
    ASSERT_TRUE(database_actions_contains_k(k4));  \
    ASSERT_TRUE(database_actions_contains_k(k5));  \
    ASSERT_TRUE(database_actions_contains_k(k6));  \
} while(0);\
if ((N)==2) do {\
    ASSERT_TRUE(database_actions_contains_k(k1));  \
    ASSERT_FALSE(database_actions_contains_k(k2)); \
    ASSERT_TRUE(database_actions_contains_k(k3));  \
    ASSERT_TRUE(database_actions_contains_k(k4));  \
    ASSERT_TRUE(database_actions_contains_k(k5));  \
    ASSERT_TRUE(database_actions_contains_k(k6));  \
} while(0);\
if ((N)==3) do {\
    ASSERT_TRUE(database_actions_contains_k(k1));  \
    ASSERT_TRUE(database_actions_contains_k(k2));  \
    ASSERT_FALSE(database_actions_contains_k(k3)); \
    ASSERT_TRUE(database_actions_contains_k(k4));  \
    ASSERT_TRUE(database_actions_contains_k(k5));  \
    ASSERT_TRUE(database_actions_contains_k(k6));  \
} while(0);\
if ((N)==4) do {\
    ASSERT_TRUE(database_actions_contains_k(k1));  \
    ASSERT_TRUE(database_actions_contains_k(k2));  \
    ASSERT_TRUE(database_actions_contains_k(k3));  \
    ASSERT_FALSE(database_actions_contains_k(k4)); \
    ASSERT_TRUE(database_actions_contains_k(k5));  \
    ASSERT_TRUE(database_actions_contains_k(k6));  \
} while(0);\
if ((N)==5) do {\
    ASSERT_TRUE(database_actions_contains_k(k1));  \
    ASSERT_TRUE(database_actions_contains_k(k2));  \
    ASSERT_TRUE(database_actions_contains_k(k3));  \
    ASSERT_TRUE(database_actions_contains_k(k4));  \
    ASSERT_FALSE(database_actions_contains_k(k5)); \
    ASSERT_TRUE(database_actions_contains_k(k6));  \
} while(0);\
if ((N)==6) do {\
    ASSERT_TRUE(database_actions_contains_k(k1));  \
    ASSERT_TRUE(database_actions_contains_k(k2));  \
    ASSERT_TRUE(database_actions_contains_k(k3));  \
    ASSERT_TRUE(database_actions_contains_k(k4));  \
    ASSERT_TRUE(database_actions_contains_k(k5));  \
    ASSERT_FALSE(database_actions_contains_k(k6)); \
} while(0);\

#define CONTAINS_KEY_1(K1) PROTECT(\
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
)

#define CONTAINS_KEY_2(K1, K2) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
)

#define CONTAINS_KEY_3(K1, K2, K3) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
ASSERT_TRUE(database_actions_contains_k(k ## K3));\
)

#define CONTAINS_KEY_4(K1, K2, K3, K4) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
ASSERT_TRUE(database_actions_contains_k(k ## K3));\
ASSERT_TRUE(database_actions_contains_k(k ## K4));\
)

#define CONTAINS_KEY_5(K1, K2, K3, K4, K5) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
ASSERT_TRUE(database_actions_contains_k(k ## K3));\
ASSERT_TRUE(database_actions_contains_k(k ## K4));\
ASSERT_TRUE(database_actions_contains_k(k ## K5));\
)

#define CONTAINS_KEY_6(K1, K2, K3, K4, K5, K6) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
ASSERT_TRUE(database_actions_contains_k(k ## K3));\
ASSERT_TRUE(database_actions_contains_k(k ## K4));\
ASSERT_TRUE(database_actions_contains_k(k ## K5));\
ASSERT_TRUE(database_actions_contains_k(k ## K6));\
)

#define CONTAINS_NOT_KEY_1(K1) PROTECT(\
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
)

#define CONTAINS_NOT_KEY_2(K1, K2) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
)

#define CONTAINS_NOT_KEY_3(K1, K2, K3) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
ASSERT_FALSE(database_actions_contains_k(k ## K3));\
)

#define CONTAINS_NOT_KEY_4(K1, K2, K3, K4) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
ASSERT_FALSE(database_actions_contains_k(k ## K3));\
ASSERT_FALSE(database_actions_contains_k(k ## K4));\
)

#define CONTAINS_NOT_KEY_5(K1, K2, K3, K4, K5) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
ASSERT_FALSE(database_actions_contains_k(k ## K3));\
ASSERT_FALSE(database_actions_contains_k(k ## K4));\
ASSERT_FALSE(database_actions_contains_k(k ## K5));\
)

#define CONTAINS_NOT_KEY_6(K1, K2, K3, K4, K5, K6) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
ASSERT_FALSE(database_actions_contains_k(k ## K3));\
ASSERT_FALSE(database_actions_contains_k(k ## K4));\
ASSERT_FALSE(database_actions_contains_k(k ## K5));\
ASSERT_FALSE(database_actions_contains_k(k ## K6));\
)

#define REMOVE(K) PROTECT(\
ASSERT_TRUE(database_actions_remove_k(k ## K));\
)

#define INSERT_KV(K) PROTECT(\
ASSERT_TRUE(database_actions_insert_kv(k ## K, &s ## K));\
)

#define CONTAINS_KEY_VALUE_1(K1) PROTECT(\
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
)

#define CONTAINS_KEY_VALUE_2(K1, K2) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
)

#define CONTAINS_KEY_VALUE_3(K1, K2, K3) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
ASSERT_TRUE(database_actions_contains_k(k ## K3));\
)

#define CONTAINS_KEY_VALUE_4(K1, K2, K3, K4) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
ASSERT_TRUE(database_actions_contains_k(k ## K3));\
ASSERT_TRUE(database_actions_contains_k(k ## K4));\
)

#define CONTAINS_KEY_VALUE_5(K1, K2, K3, K4, K5) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
ASSERT_TRUE(database_actions_contains_k(k ## K3));\
ASSERT_TRUE(database_actions_contains_k(k ## K4));\
ASSERT_TRUE(database_actions_contains_k(k ## K5));\
)

#define CONTAINS_KEY_VALUE_6(K1, K2, K3, K4, K5, K6) PROTECT( \
ASSERT_TRUE(database_actions_contains_k(k ## K1));\
ASSERT_TRUE(database_actions_contains_k(k ## K2));\
ASSERT_TRUE(database_actions_contains_k(k ## K3));\
ASSERT_TRUE(database_actions_contains_k(k ## K4));\
ASSERT_TRUE(database_actions_contains_k(k ## K5));\
ASSERT_TRUE(database_actions_contains_k(k ## K6));\
)

#define CONTAINS_NOT_KEY_VALUE_1(K1) PROTECT(\
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
)

#define CONTAINS_NOT_KEY_VALUE_2(K1, K2) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
)

#define CONTAINS_NOT_KEY_VALUE_3(K1, K2, K3) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
ASSERT_FALSE(database_actions_contains_k(k ## K3));\
)

#define CONTAINS_NOT_KEY_VALUE_4(K1, K2, K3, K4) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
ASSERT_FALSE(database_actions_contains_k(k ## K3));\
ASSERT_FALSE(database_actions_contains_k(k ## K4));\
)

#define CONTAINS_NOT_KEY_VALUE_5(K1, K2, K3, K4, K5) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
ASSERT_FALSE(database_actions_contains_k(k ## K3));\
ASSERT_FALSE(database_actions_contains_k(k ## K4));\
ASSERT_FALSE(database_actions_contains_k(k ## K5));\
)

#define CONTAINS_NOT_KEY_VALUE_6(K1, K2, K3, K4, K5, K6) PROTECT( \
ASSERT_FALSE(database_actions_contains_k(k ## K1));\
ASSERT_FALSE(database_actions_contains_k(k ## K2));\
ASSERT_FALSE(database_actions_contains_k(k ## K3));\
ASSERT_FALSE(database_actions_contains_k(k ## K4));\
ASSERT_FALSE(database_actions_contains_k(k ## K5));\
ASSERT_FALSE(database_actions_contains_k(k ## K6));\
)

#define CONTAINS_ALL_KEY_VALUE CONTAINS_KEY_VALUE_6(1,2,3,4,5,6);

#endif //KEY_VALUE_DATABASE_MACRO_UTILS_H
