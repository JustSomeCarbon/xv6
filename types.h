typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
// lock type struct for thread system
typedef struct lock_t {
    uint lock_use;
} lock_t;
