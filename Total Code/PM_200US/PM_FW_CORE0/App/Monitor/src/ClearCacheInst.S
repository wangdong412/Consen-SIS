
#define r0  0
#define r1  1
#define r2  2
#define r3  3
#define r4  4
#define r5  5
#define r6  6
#define r7  7
#define r8  8
#define r9  9
#define r10 10
#define r11 11
#define r12 12
#define r13 13
#define r14 14
#define r15 15
#define r16 16
#define r17 17
#define r18 18
#define r19 19
#define r20 20
#define r21 21
#define r22 22
#define r23 23
#define r24 24
#define r25 25
#define r26 26
#define r27 27
#define r28 28
#define r29 29
#define r30 30
#define r31 31
#define L1CSR0  1010    
#define L1CSR1  1011    

.globl disable_inst
disable_inst:
    xor     r6, r6, r6

    li      r6, 0x0000
    msync
    isync
    mtspr   L1CSR1, r6              
    isync

    li      r6, 0x0002
    msync
    isync
    mtspr   L1CSR1, r6    
    isync
    blr

.globl enable_inst
enable_inst:
    li      r6, 0x0000
    msync
    isync
    mtspr   L1CSR1, r6        
    isync

    li      r7, 0x0001
    msync
    isync
    mtspr   L1CSR1, r7        
    msync
    isync
    blr
