//
// Created by snipy on 15.03.18.
//

#ifndef LOCK_FREE_SET_ATOMIC_H
#define LOCK_FREE_SET_ATOMIC_H

#define fetch_and_increment(PTR) (__sync_fetch_and_add(PTR,1))
#define fetch_and_decrement(PTR) (__sync_fetch_and_sub(PTR,1))

#define compare_and_swap(ADDR, EXPECTED_VAL, NEW_VAL)\
(__sync_bool_compare_and_swap(ADDR,EXPECTED_VAL,NEW_VAL))


#endif //LOCK_FREE_SET_ATOMIC_H
