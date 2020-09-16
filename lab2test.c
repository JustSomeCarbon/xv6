#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int pid = getpid();

  // find nice for the pid
  printf(2, "process pid %d current priority %d\n", pid, get_priority(pid));

  // set the priority for the process to another number
  int someprior = 10;
  printf(1, "changing proccess priority to %d\n", someprior);
  // set priority
  int val = set_priority(pid, someprior);

  // check that the priority was able to be set
  // if not exit from program
  if (val == -1)
    exit();

  printf(2, "process pid %d changed priority %d\n", pid, get_priority(pid));


  // out of bounds check
  int upper = 50;
  int lower = -9;

  printf(1, "setting priority to value %d\n", lower);
  val = set_priority(pid, lower);
  if (val == -1)
    exit();
  printf(2, "process pid %d set to priority %d\n", pid, get_priority(pid));

  printf(1, "setting priority to value %d\n", upper);
  val = set_priority(pid, upper);
  if (val == -1)
    exit();
  printf(2, "process pid %d set to priority %d\n", pid, get_priority(pid));

  // exit from the program
  exit();
}
