#include "types.h"
#include "stat.h"
#include "user.h"

int
testCow()
{
    // create some parent variable
    char *p = malloc(32);
    p = "testing";
    // print parent variable
    printf(1, "parent variable: %s\n", p);

    // check the number of free pages before fork
    int free_pages = getNumFreePages();
    printf(1, "free pages before fork: %d\n", free_pages);

    // create child process
    if (fork() == 0) {
        // now check that the free pages decreased
        int free_pages_child = getNumFreePages();
        // check that the pages act correctly
        if (free_pages_child >= free_pages)
            return -1;
        printf(1, "free pages after fork: %d\n", free_pages);

        // change the parent variable information
        // change testing to tasting
        *(p+1) = 'a';
        printf(1, "variable in child: %s\n", p);
    } else {
        // have the parent wait
        wait();
    }
    return 0;
}


int
main(int argc, char *argv[])
{
    printf(1, "testing\n");
    int test = testCow();
    if (test == -1) {
        printf(1, "Fail\n");
        exit();
    }
    printf(1, "Success\n");
    exit();
}
