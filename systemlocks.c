#include "types.h"
#include "defs.h"
#include "param.h"

// initialize the thread lock
int
sys_lock_init(void)
{
    // define a system lock
    lock_t* lk;
    // look for passed lock and store within lk
    // if no lock is found return from call
    if (argint(0, (int*)&lk) < 0)
        return -1;

    // initialize the lock and return
    return lock_init(lk);
}


// aqcuire the lock for the calling thread
int
sys_lock_acquire(void)
{
    // define a system lock
    lock_t* lk;
    // look for a passed lock and store within lk
    // if no lock is found return from call
    if (argint(0, (int*)&lk) < 0)
        return -1;
    
    // try acquiring the lock and return
    return lock_acquire(lk);
}


// release the lock from the calling thread
int
sys_lock_release(void)
{
    // define a system lock
    lock_t* lk;
    // look for a passed lock and store within lk
    // if no lock is found return from call
    if (argint(0, (int*)&lk) < 0)
        return -1;

    // release the lock and return
    return lock_release(lk);
}
