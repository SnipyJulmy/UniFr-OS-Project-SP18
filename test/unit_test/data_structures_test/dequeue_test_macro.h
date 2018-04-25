//
// Created by snipy on 25.04.18.
//

#ifndef KEY_VALUE_DATABASE_DEQUEUE_TEST_MACRO_H
#define KEY_VALUE_DATABASE_DEQUEUE_TEST_MACRO_H

#define VALUE(ID_INT) int v##ID_INT = ID_INT; int* p##ID_INT = &v##ID_INT;

#define ADD(ID_INT) do {dequeue->add(dequeue,p##ID_INT);} while(0);
#define ADD_FIRST(ID_INT) do {dequeue->add_first(dequeue,p##ID_INT);} while(0);
#define PEEK_INT(ID_INT) (*(int*)(dequeue->peek_first(dequeue)))
#define AT_FIRST_V(ID_INT) ASSERT_TRUE(PEEK_INT(ID_INT) == v##ID_INT);
#define AT_FIRST_P(ID_INT) ASSERT_TRUE(dequeue->peek_first(dequeue) == p##ID_INT);
#define AT_FIRST(ID_INT) do {AT_FIRST_P(ID_INT); AT_FIRST_V(ID_INT);}while(0);
#define ASSERT_SIZE(EXPECTED_SIZE) do{\
    if((EXPECTED_SIZE) == 0)\
        ASSERT_TRUE(dequeue->is_empty(dequeue));\
    else \
        ASSERT_FALSE(dequeue->is_empty(dequeue));\
    ASSERT_TRUE(dequeue->size == (EXPECTED_SIZE));}while(0);

#endif //KEY_VALUE_DATABASE_DEQUEUE_TEST_MACRO_H
