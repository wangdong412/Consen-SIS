#ifndef __IO_H__
#define __IO_H__
#include <libc/cagboot.h>

/*
 * this file should be included in source file 
 *
 */

/*
 * 8, 16 and 32 bit, big and little endian I/O operations, with barrier.
 *
 * Read operations have additional twi & isync to make sure the read
 * is actually performed (i.e. the data has come back) before we start
 * executing any following instructions.
 */
static __inline__ uint8_t in_8(const volatile unsigned char  *addr)
{
    uint8_t ret;

    __asm__ __volatile__(
        "sync; lbz%U1%X1 %0,%1;\n"
        "twi 0,%0,0;\n"
        "isync" : "=r" (ret) : "m" (*addr));
    return ret;
}

static __inline__ void out_8(volatile unsigned char  *addr, uint8_t val)
{
    __asm__ __volatile__("sync;\n"
                         "stb%U0%X0 %1,%0;\n"
                         : "=m" (*addr)
                         : "r" (val));
}

static __inline__ uint16_t in_le16(const volatile unsigned short  *addr)
{
    uint16_t ret;

    __asm__ __volatile__("sync; lhbrx %0,0,%1;\n"
                         "twi 0,%0,0;\n"
                         "isync" : "=r" (ret) :
                         "r" (addr), "m" (*addr));
    return ret;
}

static __inline__ uint16_t in_be16(const volatile unsigned short  *addr)
{
    uint16_t ret;

    __asm__ __volatile__("sync; lhz%U1%X1 %0,%1;\n"
                         "twi 0,%0,0;\n"
                         "isync" : "=r" (ret) : "m" (*addr));
    return ret;
}


static __inline__ void out_le16(volatile unsigned short  *addr, uint16_t val)
{
    __asm__ __volatile__("sync; sthbrx %1,0,%2" : "=m" (*addr) :
                         "r" (val), "r" (addr));
}

static __inline__ void out_be16(volatile unsigned short  *addr, uint16_t val)
{
    __asm__ __volatile__("sync; sth%U0%X0 %1,%0" : "=m" (*addr) : "r" (val));
}

static __inline__ uint32_t in_le32(const volatile unsigned  *addr)
{
    uint32_t ret;

    __asm__ __volatile__("sync; lwbrx %0,0,%1;\n"
                         "twi 0,%0,0;\n"
                         "isync" : "=r" (ret) :
                         "r" (addr), "m" (*addr));
    return ret;
}

static __inline__ uint32_t in_be32(const volatile unsigned  *addr)
{
    uint32_t ret;

    __asm__ __volatile__("sync; lwz%U1%X1 %0,%1;\n"
                         "twi 0,%0,0;\n"
                         "isync" : "=r" (ret) : "m" (*addr));
    return ret;
}

static __inline__ void out_le32(volatile unsigned  *addr, uint32_t val)
{
    __asm__ __volatile__("sync; stwbrx %1,0,%2" : "=m" (*addr) :
                         "r" (val), "r" (addr));
}

static __inline__ void out_be32(volatile unsigned  *addr, uint32_t val)
{
    __asm__ __volatile__("sync; stw%U0%X0 %1,%0" : "=m" (*addr) : "r" (val));
}
#endif
