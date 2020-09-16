#include "types.h"
#include "stat.h"
#include "user.h"
#include "proc.h"
#include "defs.h"


int
main(void)
{
  int pid;
  // check if the pid was given as a parameter
  // if not return -1
  if (argint(0, &pid) < 0)
    return -1;

  // obtain the process from the process table
  struct proc* p = findprocess(pid);

  // check that the process was found
  // if not return -1
  if (p == (void *)0)
    return -1;

  // return the priority of the process
  return p->pnice;
}
