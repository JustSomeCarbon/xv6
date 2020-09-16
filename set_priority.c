#include "types.h"
#include "stat.h"
#include "user.h"
#include "proc.h"
#include "defs.h"
#include "defs.h"
#include "prio.h"


int
set_priority(void)
{
  int pid, pri;
  // check pid and priority were passed
  // if not return -1
  if (argint(0, &pid) < 0)
    return -1;

  if (argint(1, &pri) < 0)
    return -1;

  // check the priority is within acceptable range
  // if not set to range bounds
  if (pri < 0)
    pri = 0;

  if (pri > 39)
    pri = 39;

  // find the process of the given pid
  struct proc* p = findprocess(pid);

  // check that the process was found
  // if not return -1
  if (p == (void *)0)
    return -1;

  // set the new priority of the process
  p->pnice = pri;
  return 0;
}
