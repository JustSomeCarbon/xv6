#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char* argv[])
{
  int pid, nice;
  // check that pid and nice where given
  if (argc < 3 || argc > 3) {
    printf(0, "error: nice [pid] [priority]\n");
    //printf(1, "args given: %d\n", argc);
    exit();
  }

  // set variables
  pid = atoi(argv[1]);
  nice = atoi(argv[2]);
  // set the nice value
  int n = set_nice(pid, nice);
  if (n == 0) {
    printf(2, "process %d set to %d\n", pid, nice);
  } else {
    printf(1, "unable to find process %d\n", pid);
  }
  
  exit();
}
