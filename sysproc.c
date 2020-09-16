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

// return the result of cps() function
int
sys_cps(void)
{
  return cps();
}

//return the resulting priority of the pid given
int
sys_get_priority(void)
{
  int pid;
  // check a pid was given as parameter
  // if not return -1
  if (argint(0, &pid) < 0)
    return -1;

  // obtain the process from the process table
  struct proc *p = findprocess(pid);

  // return the priority of the proccess 
  return p->pnice;
}


// set the priority of the given process
int
sys_set_priority(void)
{
  int pid, pri;
  // check pid and priority were passed
  // if not return -1
  if (argint(0, &pid) < 0)
    return -1;

  if (argint(1, &pri) < 0)
    return -1;

  // check that priority is within accepted range
  // if not set to range bounds
  if (pri < 0)
    pri = 0;

  if (pri > 39)
    pri = 39;

  // find the process of the given pid
  struct proc *p = findprocess(pid);

  // set the priority of the new process
  p->pnice = pri;

  return 0;
}


/*
int
sys_get_priority(void)
{
  return get_priority();
}


int
sys_set_priority(void)
{
  return set_priority();
}
*/
