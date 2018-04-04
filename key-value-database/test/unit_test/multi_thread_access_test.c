//
// Created by snipy on 21.03.18.
//

#include <malloc.h>
#include <stdlib.h>
#include "../lib/ctest/ctest.h"
#include "../../src/server/database/lock_free_set/lock_free_set.h"
#include "../../src/server/database/lock_free_set/lock_free_set_hash.h"

#define ARRAY_LENGTH 10
#define NB_LOOP_ITERATION 10000

static int nextInt()
{
    return rand();
};

static int nextIntBound(int max)
{
    return rand() % max;
}

static uint32_t nextIdx()
{
    return (uint32_t) (rand() % ARRAY_LENGTH);
};

CTEST(multi_thread_test, simple_add_remove)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);
    #pragma omp parallel default(none) num_threads(8) shared(set)
    {
        // const int thread_num = omp_get_thread_num();
        // generate random v
        int* p = malloc(ARRAY_LENGTH * sizeof(int));
        int idx;
        for (int i = 0; i < ARRAY_LENGTH; i++)
        {
            *p = nextInt();
        }

        for (int i = 0; i < NB_LOOP_ITERATION; i++)
        {
            idx = nextIdx();
            if (set->add(set, &(p[idx])))
            {
                ASSERT_TRUE(set->contains(set, &(p[idx])));
                ASSERT_TRUE(set->remove(set, &(p[idx])));
            }
            else
            {
                ASSERT_TRUE(false);
            }
        }

        free(p);
    }
}

CTEST(multi_thread_test, simple_add_remove_from_key)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);
    #pragma omp parallel default(none) num_threads(8) shared(set)
    {
        // const int thread_num = omp_get_thread_num();
        // generate random v
        int* p = malloc(ARRAY_LENGTH * sizeof(int));
        int idx;
        for (int i = 0; i < ARRAY_LENGTH; i++)
        {
            *p = nextInt();
        }

        for (int i = 0; i < NB_LOOP_ITERATION; i++)
        {
            idx = nextIdx();
            if (set->add(set, &(p[idx])))
            {
                uint32_t key = lock_free_set_hash(&(p[idx]));
                ASSERT_TRUE(set->contains_from_key(set, key));
                ASSERT_TRUE(set->remove_from_key(set, key));
            }
            else
            {
                ASSERT_TRUE(false);
            }
        }

        free(p);
    }
}

CTEST(multi_thread_test, simple_add_from_key_remove_from_key)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);
    #pragma omp parallel default(none) num_threads(8) shared(set)
    {
        // const int thread_num = omp_get_thread_num();
        // generate random v
        int* p = malloc(ARRAY_LENGTH * sizeof(int));
        uint32_t idx;
        for (int i = 0; i < ARRAY_LENGTH; i++)
        {
            *p = nextInt();
        }

        for (int i = 0; i < NB_LOOP_ITERATION; i++)
        {
            idx = nextIdx();
            if (set->add_with_key(set, idx, &(p[idx])))
            {
                ASSERT_TRUE(set->contains_from_key(set, idx));
                set->remove_from_key(set, idx);
            }
        }
        free(p);
    }
    set->free(set);
}
