kalloc.c
-------------------
Within kalloc.c is the structure which holds the page information.
Therefore, we added to the page structure a variable to hold free
pages within the system, and an array variable to hold reference counters
for each page within the system. For the free pages, when a page was freed
from use we added 1 to the free pages variable counter. For the reference
counters, every time a new page was initialized from a fork 1 was added
to that reference counter within the variable array.
Bellow all of the supplied functions I added the system call getNumFreePages
which returns the number of free pages within the system. bellow that,
I added the helper functions for the page reference counter variable with
proper locking and unlocking.

vm.c
-------------------
Within the file vm.c we added the functions copyuvm_cow and handle_pgflt
at the bottom of the file. within copyuvm_cow, when a new page is initialized
from a fork now, instead of making a new page for the child and copying everything
over, the parent and the child now chare the information until a write is needed by
one the parent or the child. handle_pgflt handles if a page fault occurs within the
system.

testcow.c
-------------------
Added a test function for the system, the function creates a new variable, prints the
available pages, then creates a child by fork and checks the free pages again, making sure
a free page was allocated. The variable is changed within the child and printed again.
On failure the function return 'fail'.

MakeFile
-------------------
added all the necessary changes so that the kernel is able to be made.


BUGS
-------------------
When the system is made and ran, for some reason it does not load, it
remains on the line 'Booting from hard disk' and doesnt progress.
I played around with debugging for a while, and changed a lot but am unable
to adequately figure out why this is happaning. I ran out of time to work on
this and must move on. This has been a fun lab, I wish I was not so busy so I
was able to work on it more.
