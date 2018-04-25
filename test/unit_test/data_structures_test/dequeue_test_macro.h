//
// Created by snipy on 25.04.18.
//

#ifndef KEY_VALUE_DATABASE_DEQUEUE_TEST_MACRO_H
#define KEY_VALUE_DATABASE_DEQUEUE_TEST_MACRO_H

#define VALUE(ID_INT) int v##ID_INT = ID_INT; int* p##ID_INT = &v##ID_INT;

#define AT_FIRST_V(ID_INT) ASSERT_TRUE(PEEK_INT(ID_INT) == v##ID_INT);
#define AT_FIRST_P(ID_INT) ASSERT_TRUE(dequeue->peek_first(dequeue) == p##ID_INT);
#define AT_FIRST(ID_INT) do {AT_FIRST_P(ID_INT); AT_FIRST_V(ID_INT);}while(0);

#define ADD(ID_INT) do {dequeue->add(dequeue,p##ID_INT);} while(0);
#define ADD_FIRST(ID_INT) do {dequeue->add_first(dequeue,p##ID_INT);} while(0);
// add 1,2,3,4,5,6... so the stack should be 6->5->4->3->2->1
#define ADD_ALL do{\
    ADD_FIRST(1);AT_FIRST(1);\
    ADD_FIRST(2);AT_FIRST(2);\
    ADD_FIRST(3);AT_FIRST(3);\
    ADD_FIRST(4);AT_FIRST(4);\
    ADD_FIRST(5);AT_FIRST(5);\
    ADD_FIRST(6);AT_FIRST(6);\
    } while(0);

#define REMOVE(ID_INT) do {dequeue->remove(dequeue,p##ID_INT);} while(0);
#define REMOVE_FIRST_P ((int*)dequeue->remove_first(dequeue))
#define REMOVE_FIRST_V (*(int*)dequeue->remove_first(dequeue))
#define REMOVE_LAST_P ((int*)dequeue->remove_last(dequeue))
#define REMOVE_LAST_V (*(int*)dequeue->remove_last(dequeue))

#define PEEK_INT(ID_INT) (*(int*)(dequeue->peek_first(dequeue)))
#define ASSERT_SIZE(EXPECTED_SIZE) do{\
    if((EXPECTED_SIZE) == 0)\
        ASSERT_TRUE(dequeue->is_empty(dequeue));\
    else \
        ASSERT_FALSE(dequeue->is_empty(dequeue));\
    ASSERT_TRUE(dequeue->size == (EXPECTED_SIZE));}while(0);

#define VALUES_1_6 VALUE(1);VALUE(2);VALUE(3);VALUE(4);VALUE(5);VALUE(6);

#endif //KEY_VALUE_DATABASE_DEQUEUE_TEST_MACRO_H
