// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;

  // Keep track of the free pages in table
  int free_pages;
  // keep track of refferences on the each page
  int page_ref[PHYSTOP];
} kmem;

void add_page_ref(int);
void sub_page_ref(int);
int check_valid_ref(int);


// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  // initialize number of free pages to 0
  // initially, we will count up later.
  kmem.free_pages = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);

  // for each page from start address given to stop address
  // free the pages and initialize page references
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE) {
    // initialize the number of page references to 0
    // change ciretual address to physical address
    kmem.page_ref[V2P(p)] = 0;
    kfree(p);
  }
}


//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // initialize the lock
  /*
  if (kmem.use_lock) {
    acquire(&kmem.lock);
  }
  */

  int page_ref_count = kmem.page_ref[V2P(v)];

  // check if there are more than one references on a
  // single page, if there are, reduce the number by 1
  // otherwise free the page space.
  if (page_ref_count == 0 ) {
    //get lock
    if (kmem.use_lock)
        acquire(&kmem.lock);

    // there is no reference
    memset(v, 1, PGSIZE);
    r = (struct run *)v;
    r->next = kmem.freelist;
    kmem.freelist = r;

    // release lock
    if (kmem.use_lock)
        release(&kmem.lock);
  }

  if (page_ref_count == 1) {
    // subtract reference on page
    // add a free page
    sub_page_ref(V2P(v));

    // acquie lock
    if (kmem.use_lock)
        acquire(&kmem.lock);

    kmem.free_pages++;
    memset(v, 1, PGSIZE);
    r = (struct run *)v;
    r->next = kmem.freelist;
    kmem.freelist = r;

    // release lock
    if (kmem.use_lock)
        release(&kmem.lock);
  }

  if (page_ref_count > 1) {
    // subtract from reference count, but
    // dont add to free pages
    sub_page_ref(V2P(v));
  }

  /*
  if (page_ref_count > 1) {
    // multiple references on page,
    // subtract a reference from the page
    kmem.page_ref[V2P(v)]--;

  } else {
    // remove the remaining reference from the page and
    // add another free page to the memory struct
    kmem.page_ref[V2P(v)]--;
    kmem.free_pages++;
    // Fill with junk to catch dangling refs
    memset(v, 1, PGSIZE);
    //operations on run struct 
    r = (struct run*)v;
    r->next = kmem.freelist;
    kmem.freelist = r;
  }

  // release the lock
  if(kmem.use_lock)
    release(&kmem.lock);
  */
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  // aqcuire the lock for the system
  /*
  if(kmem.use_lock)
    acquire(&kmem.lock);
  */

  // find the next free page available
  r = kmem.freelist;

  // if a free page is found allocate it
  // set page values
  if(r) {
    // add a reference for the page
    // remove a page from free pages
    add_page_ref(V2P((char *)r));
    // acquire lock
    if (kmem.use_lock)
        acquire(&kmem.lock);

    kmem.free_pages--;
    //kmem.page_ref[V2P((char *)r)]++;
    // set the freelist to the next page
    kmem.freelist = r->next;
  }

  // release the lock
  if(kmem.use_lock)
    release(&kmem.lock);

  return (char*)r;
}

/* -- System Call -- */

// getNumFreePages returns the number of free pages
// within the page table
int getNumFreePages() {
    // first obtain the lock
    if (kmem.use_lock) {
        acquire(&kmem.lock);
    }
    // obtain the free pages
    int free = kmem.free_pages;
    //return the lock
    if (kmem.use_lock) {
        release(&kmem.lock);
    }
    // return the free pages
    return free;
}


/* -- helper functions -- */

// Checks if the given page address is valid
int
check_valid_ref(int page)
{
    // check that the given page is valid
    // the page must be within allowed memory
    // to be valid
    // return 1 if it is, 0 otherwise
    return ((page < PHYSTOP) || ((uint)V2P(end) > page));
}


// Add a reference to a given page
void
add_page_ref(int page)
{
    // check page is valid
    if (!check_valid_ref(page))
        panic("add_page_ref");

    // acquire the lock
    if (kmem.use_lock)
        acquire(&kmem.lock);

    // add a reference for the given page
    kmem.page_ref[page] += 1;

    // release lock
    if (kmem.use_lock)
        release(&kmem.lock);
}


// Subtract a page reference from given page if possible
void
sub_page_ref(int page)
{
    // check page is valid
    if (!check_valid_ref(page))
        panic("sub_page_ref");

    // acquire the lock
    if (kmem.use_lock)
        acquire(&kmem.lock);

    int ref = kmem.page_ref[page];

    // check that a reference can be subtracted
    // if not, panic
    if (ref == 0) {
        panic("sub_page_ref");
    }
    if (ref >= 1) {
        // subtract 1 from page references
        kmem.page_ref[page] -= 1;
    }

    // release lock
    if (kmem.use_lock)
        release(&kmem.lock);
}





