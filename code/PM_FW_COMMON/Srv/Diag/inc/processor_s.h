
#ifndef _PROCESSOR_S_H
#define _PROCESSOR_S_H

#define MASK_SDR1_RES_BITS_HI                    (0xFFFF)
#define MASK_SDR1_RES_BITS_LO                    (0x01FF)

#define MASK_HID2_DWLCK_BITS_HI                  (0xFFFF)
#define MASK_HID2_DWLCK_BITS_LO                  (0xFF1F)

#define MASK_HID0_ICFI_BITS_HI                   (0x0000)
#define MASK_HID0_ICFI_BITS_LO                   (0x0800)

// MSR bits
#define MSR_POW_SHIFT                            (18)
#define MSR_POW                                  (1<<MSR_POW_SHIFT)          // Enable Power Management
#define MSR_FP_SHIFT                             (13)
#define MSR_FP                                   (1<<MSR_FP_SHIFT)           // Floating Point enable
#define MSR_ME_SHIFT                             (12)
#define MSR_ME                                   (1<<MSR_ME_SHIFT)           // Machine Check Enable
#define MSR_EE_SHIFT                             (15)
#define MSR_EE                                   (1<<MSR_EE_SHIFT)           // External Interrupt Enable
#define MSR_IR_SHIFT                             (5)
#define MSR_IR                                   (1<<MSR_IR_SHIFT)           // Instruction Relocate
#define MSR_IP_SHIFT                             (6)
#define MSR_IP                                   (1<<MSR_IP_SHIFT)           // Exception prefix 0x000/0xFFF
#define MSR_DR_SHIFT                             (4)
#define MSR_DR                                   (1<<MSR_DR_SHIFT)           // Data Relocate
#define MSR_RI_SHIFT                             (4)
#define MSR_RI                                   (1<<MSR_RI_SHIFT)           // Recoverable Exception

// HID0 bits
#define HID0_DCE_SHIFT                           (14)
#define HID0_DCE                                 (1<<HID0_DCE_SHIFT)         // Dcache enable
#define HID0_DCFI_SHIFT                          (10)
#define HID0_DCFI                                (1<<HID0_DCFI_SHIFT)        // Data Cache Flash Invalidate
#define HID0_DLOCK_SHIFT                         (12)
#define HID0_DLOCK                               (1<<HID0_DLOCK_SHIFT)       // Data Cache Lock
#define HID0_ICE_SHIFT                           (15)
#define HID0_ICE                                 (1<<HID0_ICE_SHIFT)         // Instruction Cache enable
#define HID0_ICFI_SHIFT                          (11)
#define HID0_ICFI                                (1<<HID0_DCFI_SHIFT)        // Instruction Cache Flash Invalidate
#define HID0_ILOCK_SHIFT                         (13)
#define HID0_ILOCK                               (1<<HID0_ILOCK_SHIFT)       // Instruction Cache Lock

// Special Function Register
#define XER                  (1)
#define LR                   (8)
#define CTR                  (9)
#define DSISR                (18)
#define DAR                  (19)
#define DEC                  (22)
#define SDR1                 (25)
#define SRR0                 (26)
#define SRR1                 (27)
#define VRSAVE               (256)
#define TBL                  (268)
#define TBU                  (269)
#define SPRG0                (272)
#define SPRG1                (273)
#define SPRG2                (274)
#define SPRG3                (275)
#define SPRG4                (276)
#define SPRG5                (277)
#define SPRG6                (278)
#define SPRG7                (279)
#define EAR                  (282)
#define wTLB                 (284)
#define wTBU                 (285)
#define PVR                  (287)
#define IBAT0U               (528)
#define IBAT0L               (529)
#define IBAT1U               (530)
#define IBAT1L               (531)
#define IBAT2U               (532)
#define IBAT2L               (533)
#define IBAT3U               (534)
#define IBAT3L               (535)
#define DBAT0U               (536)
#define DBAT0L               (537)
#define DBAT1U               (538)
#define DBAT1L               (539)
#define DBAT2U               (540)
#define DBAT2L               (541)
#define DBAT3U               (542)
#define DBAT3L               (543)
#define IBAT4U               (560)
#define IBAT4L               (561)
#define IBAT5U               (562)
#define IBAT5L               (563)
#define IBAT6U               (564)
#define IBAT6L               (565)
#define IBAT7U               (566)
#define IBAT7L               (567)
#define DBAT4U               (568)
#define DBAT4L               (569)
#define DBAT5U               (570)
#define DBAT5L               (571)
#define DBAT6U               (572)
#define DBAT6L               (573)
#define DBAT7U               (574)
#define DBAT7L               (575)
#define UMMCR2               (928)
#define UPMC5                (929)
#define UPMC6                (930)
#define UMMCR0               (936)
#define UPMC1                (937)
#define UPMC2                (938)
#define USIAR                (939)
#define UMMCR1               (940)
#define UPMC3                (941)
#define UPMC4                (942)
#define MMCR2                (944)
#define PMC5                 (945)
#define PMC6                 (946)
#define BAMR                 (951)
#define MMCR0                (952)
#define PMC1                 (953)
#define PMC2                 (954)
#define SIAR                 (955)
#define MMCR1                (956)
#define PMC3                 (957)
#define PMC4                 (958)
#define TLBMISS              (980)
#define PTEHI                (981)
#define PTELO                (982)
#define L3PM                 (983)
#define L3ITCR0              (984)
#define L3OHCR               (1000)
#define HID0                 (1008)
#define HID1                 (1009)
#define IABR                 (1010)
#define HID2                 (1011)
#define DABR                 (1013)
#define MSSCR0               (1014)
#define MSSSR0               (1015)
#define LDSTCR               (1016)
#define L2CR                 (1017)
#define L3CR                 (1018)
#define ICTC                 (1019)
#define PIR                  (1023)

#endif/*  _PROCESSOR_S_H */
