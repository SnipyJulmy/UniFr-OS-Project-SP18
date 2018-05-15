//
// Created by snipy on 21.03.18.
//

#include <malloc.h>
#include <stdlib.h>
#include "../lib/ctest/ctest.h"
#include "../../src/data_structures/parallel/lock_free_set/lock_free_set.h"
#include "../../src/data_structures/parallel/lock_free_set/lock_free_set_hash.h"
#include "../../src/debug.h"
#include "utils/random.h"

#define ARRAY_LENGTH 10
#define NB_LOOP_ITERATION 100//0000
#define RND_CHAR_SIZE 100
#define NB_ITEMS_TO_INSERT 100//0000

static int nextInt()
{
    return rand();
};

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
            Key key = set->item_hashcode(&(p[idx]));
            if (set->add_with_key(set, key, &(p[idx])))
            {
                ASSERT_TRUE(set->contains_from_key(set, key));
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
    set->destroy(set);
}

CTEST(multi_thread_test, add_read)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);
    check_mem_and_exit(set);

    // each thread would add the same value, store the key and retrieve their own values
    #pragma omp parallel default(none) num_threads(8) shared(set, stderr)
    {
        // Dequeue to track inserted item
        Dequeue* inserted_items = key_value_database_dequeue_create(
                sizeof(KV),
                key_value_database_KV_compare,
                key_value_database_KV_free
        );

        check_mem(inserted_items, {
            goto end;
        });

        for (int i = 0; i < NB_ITEMS_TO_INSERT; i++)
        {
            // allocate memory for random string and a dequeue
            char** rnd_string = malloc(1 * sizeof(char*));
            ASSERT_NOT_NULL(rnd_string);
            rnd_string[0] = malloc(RND_CHAR_SIZE * sizeof(char));
            ASSERT_NOT_NULL(rnd_string[0]);

            char** rnd_string_cpy = malloc(1 * sizeof(char*));
            ASSERT_NOT_NULL(rnd_string_cpy);
            rnd_string_cpy[0] = malloc(RND_CHAR_SIZE * sizeof(char));
            ASSERT_NOT_NULL(rnd_string_cpy[0]);

            // generate random char and copy it
            gen_random(rnd_string[0], RND_CHAR_SIZE - 1);
            strcpy(rnd_string_cpy[0], rnd_string[0]);

            // insert the random string into the database
            Key key = set->item_hashcode(rnd_string);
            ASSERT_TRUE(set->add_with_key(set, key, rnd_string));
            ASSERT_TRUE(set->contains_from_value(set,rnd_string));
            ASSERT_TRUE(set->contains_from_value(set,rnd_string_cpy));
            ASSERT_TRUE(set->contains_from_key(set, key));
            ASSERT_TRUE(set->read(set, key) == rnd_string);
            ASSERT_TRUE(strcmp(rnd_string[0], *(char**) set->read(set, key)) == 0);
            inserted_items->add(inserted_items, key_value_database_KV_create(key, rnd_string_cpy[0]));
        }

        while (!inserted_items->is_empty(inserted_items))
        {
            KV* kv = inserted_items->remove_first(inserted_items);
            ASSERT_TRUE(set->contains_from_value(set, &kv->value));
            ASSERT_TRUE(set->contains_from_key(set, kv->key));
            ASSERT_TRUE(strcmp(kv->value, *(char**) set->read(set, kv->key)) == 0);
        }

        inserted_items->destroy(inserted_items);
        end:;
    }

    set->destroy(set);
}

CTEST(multi_thread_test, add_read_delete)
{
    Set* set = lock_free_data_create_set(lock_free_set_hash);
    check_mem_and_exit(set);

    // each thread would add the same value, store the key and retrieve their own values
    #pragma omp parallel default(none) num_threads(8) shared(set, stderr)
    {
        // Dequeue to track inserted item
        Dequeue* inserted_items = key_value_database_dequeue_create(
                sizeof(KV),
                key_value_database_KV_compare,
                key_value_database_KV_free
        );

        check_mem(inserted_items, {
            goto end;
        });

        for (int i = 0; i < NB_ITEMS_TO_INSERT; i++)
        {
            // allocate memory for random string and a dequeue
            char** rnd_string = malloc(1 * sizeof(char*));
            ASSERT_NOT_NULL(rnd_string);
            rnd_string[0] = malloc(RND_CHAR_SIZE * sizeof(char));
            ASSERT_NOT_NULL(rnd_string[0]);

            char** rnd_string_cpy = malloc(1 * sizeof(char*));
            ASSERT_NOT_NULL(rnd_string_cpy);
            rnd_string_cpy[0] = malloc(RND_CHAR_SIZE * sizeof(char));
            ASSERT_NOT_NULL(rnd_string_cpy[0]);

            // generate random char and copy it
            gen_random(rnd_string[0], RND_CHAR_SIZE - 1);
            strcpy(rnd_string_cpy[0], rnd_string[0]);

            // insert the random string into the database
            Key key = set->item_hashcode(rnd_string);
            ASSERT_TRUE(set->add_with_key(set, key, rnd_string));
            ASSERT_TRUE(set->contains_from_key(set, key));
            ASSERT_TRUE(set->read(set, key) == rnd_string);
            ASSERT_TRUE(strcmp(rnd_string[0], *(char**) set->read(set, key)) == 0);
            inserted_items->add(inserted_items, key_value_database_KV_create(key, rnd_string_cpy[0]));
        }

        while (!inserted_items->is_empty(inserted_items))
        {
            KV* kv = inserted_items->remove_first(inserted_items);
            ASSERT_TRUE(set->contains_from_key(set, kv->key));
            ASSERT_TRUE(strcmp(kv->value, *(char**) set->read(set, kv->key)) == 0);
            ASSERT_TRUE(set->remove_from_key(set, kv->key));
            ASSERT_FALSE(set->contains_from_key(set, kv->key));
        }

        inserted_items->destroy(inserted_items);
        end:;
    }

    set->destroy(set);
}
