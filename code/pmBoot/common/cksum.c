typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;
typedef unsigned int size_t;

/* This is a NOEXEC applet. Be very careful! */

#define EXIT_SUCCESS 0
uint8_t crctable[256*4]={0};
uint32_t* crc32_filltable(uint32_t *crc_table, int endian)
{
    uint32_t polynomial = endian ? 0x04c11db7 : 0xedb88320;
    uint32_t c;
    int i, j;

    if (!crc_table)
        crc_table = crctable;//malloc(256 * sizeof(uint32_t));

    for (i = 0; i < 256; i++) {
        c = endian ? (i << 24) : i;
        for (j = 8; j; j--) {
            if (endian)
                c = (c&0x80000000) ? ((c << 1) ^ polynomial) : (c << 1);
            else
                c = (c&1) ? ((c >> 1) ^ polynomial) : (c >> 1);
        }
        *crc_table++ = c;
    }

    return crc_table - 256;
}

#if 0
ssize_t safe_read(int fd, void *buf, size_t count)
{
    ssize_t n;

    do {
        n = read(fd, buf, count);
    } while (n < 0 && errno == EINTR);

    return n;
}
#endif

uint32_t crc32_block_endian1(uint32_t val, const void *buf, unsigned len, uint32_t *crc_table)
{
    const void *end = (uint8_t*)buf + len;

    while (buf != end) {
    //    printf("%x ",*(uint8_t *)buf);
        val = (val << 8) ^ crc_table[(val >> 24) ^ *(uint8_t*)buf];
        buf = (uint8_t*)buf + 1;
    }
    return val;
}


unsigned int cksum_crc32(uint32_t addr ,uint32_t len)
{
    uint32_t *crc32_table = crc32_filltable(0, 1);
    uint32_t crc;
    uint32_t length, filesize;
    int bytes_read;
    uint8_t *buf=(uint8_t *)addr;//malloc(4*1024*1024);
    int n=0;
    printf("addr %x %x len %x %d \n",addr,buf,len,len);
        
    crc=0;
    length=0;

    crc = crc32_block_endian1(crc,buf,len,crc32_table);
    length = len;

    filesize = length;

    while (length) {
        crc = (crc << 8) ^ crc32_table[(uint8_t)(crc >> 24) ^ (uint8_t)length];
        /* must ensure that shift is unsigned! */
        if (sizeof(length) <= sizeof(unsigned))
            length = (unsigned)length >> 8;
        else if (sizeof(length) <= sizeof(unsigned long))
            length = (unsigned long)length >> 8;
        else
            length = (unsigned long long)length >> 8;
    }
    crc = ~crc;

    return crc;
}
