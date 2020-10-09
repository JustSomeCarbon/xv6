#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char* argv[])
{
  int pid, nice;
  // check that pid and nice where given
  if (argc < 2 || argc > 2)
    exit();

  // set variables
  pid = atoi(argv[0]);
  nice = atoi(argv[1]);
  // set the nice value
  n = set_nice(pid, nice);
  exit();
}
