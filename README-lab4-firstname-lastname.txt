Makefile::
I had to add the files of threads_nosync.c and threads_sync.c to the user program files so
I was able to call and run them within the xv6 environment. I also added the object file
for the tlock.c file so the system can use it to look for the lock functions.

defs.h::
I added the prototypes for the thread and lock functions under their designated function
files.

proc.c::
added the thread functions to the bottom of the file (in the order: create, join, exit).
unfortunately the system traps the threads created and after the threads_nosync/sync runs
the ammount is always returned as 0. I worked on solving the problem but I have run out of
time and I was unable to solve the problem before the deadline.

proc.h::
added a flag that allows the system to identify if the current process is actually a kernel
thread. also added a stack pointer to hold information regarding the stack of the original
process for threads.

syscall.c/h::
added information regarding the new functions being added to the operating system.

sysproc.c::
added the system call functions for the lock functions, so the system can find them.

tlock.c::
The file that contains all of the funcions for the thread lock functionality that I added
to the system.

types.h::
added a new types structure to hold the thread lock information for the entire system.

user.h::
added all the new function prototypes for the user programs
