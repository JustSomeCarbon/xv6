#include "x86.h"
#include "defs.h"
#include "types.h"

// initialize the lock for the threads
// there should only be one lock within the system
int lock_init(lock_t* lk)
{
    // initialize the lock to be 0 (not in use)
    lk->lock_use = 0;
    return 0;
}


// try and acquire the lock for the current process
//thread. if the lock is already in use, set the
// process thread to sleep until the lock becomes
// available to acquire. If the lock is available,
// set the lock_use to 1.
int lock_acquire(lock_t* lk)
{
    // check that the lock is not in use, while it
    // is, sleep until it is available. If not in
    // use, set it to use and return.
    while(xchg(&lk->lock_use, 1) != 0) {
        yield();
    }

    // lock has been acquired now return
    return 0;
}


// call this once the thread is done running to release
// the acquired lock so other threads are able to run
int lock_release(lock_t* lk)
{
    // exchange the current lock_use value with 0
    xcchg(&lk->lock_use, 0);
    // the lock is now free to use, return
    return 0;
}
