#include <libc/cagboot.h>

/*
 * 8 I/O operation with barrier.
 *
 * Read operations have additional twi & isync to make sure the read
 * is actually performed (i.e. the data has come back) before we start
 * executing any following instructions.
 */
UINT8 in_8i(volatile UINT8 __iomem *addr)
{
    UINT8 ret;

    __asm__ __volatile__(
        "sync; lbz%U1%X1 %0,%1;\n"
        "twi 0,%0,0;\n"
        "isync" : "=r" (ret) : "m" (*addr));
    return ret;
}

void out_8i(volatile UINT8 __iomem *addr, UINT8 val)
{
    __asm__ __volatile__("sync;\n"
                         "stb%U0%X0 %1,%0;\n"
                         : "=m" (*addr)
                         : "r" (val));
}
