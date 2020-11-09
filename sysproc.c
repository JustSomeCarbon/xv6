#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// print current processes to terminal
int
sys_cps(void)
{
	return cps();
}

// create a new thread
int
sys_thread_create(void)
{
    // create variables to store passed information
    void *fn = 0, *stack = 0, *arg = 0;

    // check for the three arguments and store them within their pointers
    if (argptr(0, (void *)fn, sizeof(*fn)) < 0)
        return -1;
    if (argptr(1, (void *)stack, sizeof(*stack)) < 0)
        return -1;
    if (argptr(2, (void *)arg, sizeof(*arg)) < 0)
        return -1;
    // call the create thread function
    return thread_create(fn, stack, arg);
}


// join a child thread with its parent
int
sys_thread_join(void)
{
    // call the join thread function
    return thread_join();
}


// exit a thread process
int
sys_thread_exit(void)
{
    // call the exit thread function
    return thread_exit();
}


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
