typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

// define a new struct lock type
// that will hold lock information
typedef struct lock_t
{
    // int to hold wether the lock is currently in
    // use by a thread. 1 if in use, 0 if not
    int lock_use;
} lock_t;
