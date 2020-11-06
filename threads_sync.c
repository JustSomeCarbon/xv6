#include "types.h"
#include "stat.h"
#include "user.h"

struct balance {
    char name[32];
    int amount;
};

// create lock for the threads
lock_t lk;
volatile int total_balance = 0;

volatile unsigned int delay (unsigned int d) {
   unsigned int i; 
   for (i = 0; i < d; i++) {
       __asm volatile( "nop" ::: );
   }

   return i;   
}

void do_work(void *arg){
    int i; 
    int old;

    // acquire the lock for the thread
    lock_acquire(&lk);
    struct balance *b = (struct balance*) arg; 
    printf(1, "Starting do_work: s:%s\n", b->name);

    for (i = 0; i < b->amount; i++) { 
         old = total_balance;
         delay(100000);
         total_balance = old + 1;
    }
  
    printf(1, "Done s:%x\n", b->name);
    // release the lock after the work is done
    lock_release(&lk);

    thread_exit();
    return;
}

int main(int argc, char *argv[]) {
  // initialize the lock
  // for the threads to use
  lock_init(&lk);

  struct balance b1 = {"b1", 3200};
  struct balance b2 = {"b2", 2800};
 
  void *s1, *s2;
  int t1, t2, r1, r2;

  s1 = malloc(4096);
  s2 = malloc(4096);

  t1 = thread_create(do_work, s1, (void*)&b1);
  t2 = thread_create(do_work, s2, (void*)&b2); 

  r1 = thread_join();
  r2 = thread_join();
  
  printf(1, "Threads finished: (%d):%d, (%d):%d, shared balance:%d\n", 
      t1, r1, t2, r2, total_balance);

  exit();
}
