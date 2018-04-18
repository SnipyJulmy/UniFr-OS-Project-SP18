//
// Created by snipy on 18.04.18.
//

#include "../../lib/ctest/ctest.h"
#include "../../../src/data_structures/dequeue.h"

#define VALUE(ID_INT) int v##ID_INT = ID_INT; int* p##ID_INT = &v##ID_INT;

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

CTEST(dequeue, simple_usage)
{
    Dequeue* dequeue = key_value_database_dequeue_create(sizeof(int), int_cmp, int_destroy);

    VALUE(1);
    VALUE(2);
    VALUE(3);
    VALUE(4);

    dequeue->add(dequeue,p1);
    ASSERT_TRUE(dequeue->contains(dequeue,p1));

    dequeue->destroy(dequeue);
}
