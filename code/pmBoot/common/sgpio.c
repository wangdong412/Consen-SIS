/* sysGpio.c - P2020DS GPIO driver source file */

/*
modification history
--------------------
01a,31may14,Jeff.Lau  written
*/

/*
DESCRIPTION
This is the driver for the Freescale P2020 GPIO controller.  This device
has a total of 16 pins, each of which can be programmed as either an
input or output.  When programmed as an input a pin can be used as an
interrupt source.  Each pin can configed as detect interrupts as active
falling edge or level change.


SEE ALSO:

\tb "QorIQ P2020 Integrated Processor Reference Manual"
*/

/* includes */

#include <common.h>
#include <malloc.h>
/* defines */

#define ESPI_CSMODE_REV_MSB_FIRST   (1 << 29)
#define SPI_REG_READ_32(addr)         *((volatile uint32_t *)(0xffe07000+ addr))
#define SPI_REG_WRITE_32(addr, data)  *((volatile uint32_t *)(0xffe07000+ addr)) = data
#define ESPI_SPMODE0                    (0xffe07000+ 0x20)
#define SPCOM_CS(x)             ((x) << 30)
#define SPCOM_RxSKIP(x)         ((x) << 16)
#define SPCOM_TO(x)             ((x) << 27)
#define EIEIO_SYNC                    __asm__ __volatile__ (" eieio; sync")
#define TRANSONLY               1
#define SPITF                   0x10
#define SPIE                    0x04   /* eSPI event register */
#define SPMODE                  0x00
#define SPMODE0                 0x20
#define SPIE                    0x04
#define SPIM                    0x08
#define SPCOM                   0x0C
#define OK 0
#define ERROR -1


#define FLAG_A 1
#define FLAG_B 2

#define FPGA_FLAG_ADDR  0xec020000

#define FPGA_A_LEN_ADDR   0xeca40000
#define FPGA_B_LEN_ADDR   0xece40000

#define FPGA_A_DATA_ADDR    0xeca40004
#define FPGA_B_DATA_ADDR    0xece40004
/* GPIO Registers */
typedef unsigned int UINT32;
typedef signed int INT32;
typedef unsigned short UINT16;
typedef signed short INT16;
typedef unsigned char UINT8;
typedef signed char INT8;
typedef unsigned long long UINT64;
typedef signed long long INT64;

typedef volatile unsigned char VCHAR;      /* shorthand for volatile UCHAR */
typedef volatile int VINT32;     /* volatile unsigned word */
typedef volatile short VINT16;     /* volatile unsigned halfword */
typedef volatile char VINT8;      /* volatile unsigned byte */
typedef volatile unsigned int VUINT32;    /* volatile unsigned word */
typedef volatile unsigned short VUINT16;    /* volatile unsigned halfword */
typedef volatile unsigned char VUINT8;     /* volatile unsigned byte */

/*macro*/

#define GPIO_BASE                       (0xffe00000 + 0xF000)
#define GPDIR                           (GPIO_BASE + 0xC00)
#define GPODR                           (GPIO_BASE + 0xC04)
#define GPDAT                           (GPIO_BASE + 0xC08)
#define GPIER                           (GPIO_BASE + 0xC0C)
#define GPIMR                           (GPIO_BASE + 0xC10)
#define GPICR                           (GPIO_BASE + 0xC14)

#define GPIO_0                          0
#define GPIO_1                          1
#define GPIO_2                          2
#define GPIO_3                          3
#define GPIO_4                          4
#define GPIO_5                          5
#define GPIO_6                          6
#define GPIO_7                          7
#define GPIO_8                          8
#define GPIO_9                          9
#define GPIO_10                         10
#define GPIO_11                         11
#define GPIO_12                         12
#define GPIO_13                         13
#define GPIO_14                         14
#define GPIO_15                         15

#define GPIO_PIN_MAX                    15  /* 16 pins (incl. 0) */

#define GPIO_LOW                        0x00
#define GPIO_HIGH                       0x01

#define GPIO_REG_WRITE(regPtr,val)      (*((VUINT32 *)(regPtr)) = (val))
#define GPIO_REG_READ(regPtr,res)       ((res) = *(VUINT32 *)(regPtr))

#define GPIO_CODE_REENTRANT

#define GPIO_INTLOCK(key)
#define GPIO_INTUNLOCK(key)
#define GPIO_KEYDECLARE

/* GPIO pin types */

#define GPIO_OUT                        0x00008000
#define GIPO_OD                         0x00000001
#define GPIO_IN                         0x00000000
#define GPIO_INT                        0x80000000

/* Interrupt styles, these refer to actual values used in reg */

#define GPIO_INT_FAIL_TRIG              0x00010000
#define GPIO_INT_LEVEL_TRIG             0x00020000

/*******************************************************************************
*
* sysGpioLineConfig - configure a GPIO pin
*
* This routine configures a GPIO pin/line number specified in the <lineNo>
* parameter for use as either an input or output, as specified in the
* <style> parameter.
*
* Example:
*   1) Config GPIO_0 as an open drain output Pin, it should call
*
*      sysGpioLineConfig (GPIO_0, (GPIO_OUT | GIPO_OD));
*
*   2) Config GPIO_1 as an external interrupt input Pin, level change
*
*      sysGpioLineConfig (GPIO_1, (GPIO_IN | GPIO_INT | GPIO_INT_LEVEL_TRIG));
*
* RETURNS:
* 0 when line is successfully configured, else ERROR if the line number
* is out of range, or an unknown style is requested.
*
* ERRNO
*/

int sysGpioLineConfig( UINT8  lineNo, UINT32 style)
{
    VUINT32 gpioDirReg;
    VUINT32 intMaskReg;
    VUINT32 intCtrlReg;
    VUINT32 gpioOpenDrainReg;

    GPIO_KEYDECLARE;

    if (lineNo > GPIO_PIN_MAX)
        return(-1);

    GPIO_REG_READ(GPDIR, gpioDirReg);
    GPIO_REG_READ(GPODR, gpioOpenDrainReg);
    GPIO_REG_READ(GPICR, intCtrlReg);
    GPIO_REG_READ(GPIMR, intMaskReg);

    if (style & GPIO_OUT) {
        gpioDirReg |= (0x80000000 >> lineNo);

        if (style & GIPO_OD)
            gpioOpenDrainReg |= (0x80000000 >> lineNo);
        else
            gpioOpenDrainReg &= ~(0x80000000 >> lineNo);
    } else {
        gpioDirReg &= ~(0x80000000 >> lineNo);

        if (style & GPIO_INT) {
            if (style & GPIO_INT_FAIL_TRIG)
                intCtrlReg |= (0x80000000 >> lineNo);
            else if (style & GPIO_INT_LEVEL_TRIG)
                intCtrlReg &= ~(0x80000000 >> lineNo);
            else
                return(-1);

            intMaskReg &= ~(0x80000000 >> lineNo);
        }
    }

    GPIO_INTLOCK (GPIO_KEYDECLARE);

    /* Set the new style */

    GPIO_REG_WRITE(GPDIR, gpioDirReg);
    GPIO_REG_WRITE(GPODR, gpioOpenDrainReg);
    GPIO_REG_WRITE(GPICR, intCtrlReg);
    GPIO_REG_WRITE(GPIMR, intMaskReg);

    GPIO_INTUNLOCK (GPIO_KEYDECLARE);

    return(0);
}

/*******************************************************************************
*
* sysGpioLineGet - get the value of a GPIO pin
*
* This routine returns the value of a specified GPIO <lineNo> pin to the
* location specified in the <value> parameter.
*
* RETURNS:
* 0 when a pin value is successfully retrieved, else ERROR if the
* specified pin number is out of range.
*
* ERRNO
*/

int sysGpioLineGet(UINT32 * gpioData, UINT8    lineNo)
{
    if ((lineNo > GPIO_PIN_MAX) || (gpioData == NULL))
        return (-1);

    GPIO_REG_READ(GPDAT, *gpioData);

    *gpioData = (*gpioData >> (31 - lineNo)) & 0x01;

    return (0);
}

/*******************************************************************************
*
* sysGpioLineSet - set the value of a GPIO pin configured for output
*
* This routine sets the <value> of a specified GPIO <lineNo> pin number.
*
* RETURNS:
* 0 when pin is successfully set, else ERROR if the specified pin number
* is out of range.
*
* ERRNO
*/

int sysGpioLineSet( UINT8   lineNo, UINT8   value)
{
    UINT32  gpioData;

    if (lineNo > GPIO_PIN_MAX)
        return (-1);

    GPIO_REG_READ(GPDAT, gpioData);

    if (value == GPIO_HIGH) {
        gpioData |= (0x80000000 >> lineNo);
    } else if (value == GPIO_LOW) {
        gpioData &= ~(0x80000000 >> lineNo);
    }

    GPIO_REG_WRITE(GPDAT, gpioData);

    return (0);
}

/*******************************************************************************
*
* sysGpioIntStatusGet - get a GPIO (interrupt) pin status
*
* Given GPIO pin number <lineNo>, this routine returns its status to the
* location specified in the <status> parameter.
*
* RETURNS:
* 0 when status is successfully retrieved, else -1 if the specified
* pin number is out of range.
*
* ERRNO
*/

int sysGpioIntStatusGet(UINT8 lineNo,char *status)
{
    volatile UINT32 intStatusReg;

    if (lineNo > GPIO_PIN_MAX)
        return (-1);

    if (status == 0)
        return (-1);

    GPIO_REG_READ(GPIER, intStatusReg);

    *status = ((intStatusReg & (0x80000000 >> lineNo)) != 0);

    return (0);
}

/*******************************************************************************
*
* sysGpioIntStatusClear - clear a GPIO (interrupt) pin status
*
* This routine returns the GPIO pin status.
*
* RETURNS:
* 0 if status cleared, else -1 if the pin number passed is out of range.
*
* ERRNO
*/

int sysGpioIntStatusClear(UINT8 lineNo)
{
    UINT32 intStatusClr;

    if (lineNo > GPIO_PIN_MAX)
        return(-1);

    intStatusClr = (0x80000000 >> lineNo);

    /* Write 1 to clear */

    GPIO_REG_WRITE(GPIER, intStatusClr);

    return(0);
}

/*******************************************************************************
*
* sysGpioReset - reset the GPIO controller
*
* This routine resets the pins of the GPIO controller to known default settings.
* The settings (defined in p2020Gpio.h) modify how the pins are programmed
* and what value they are set at. It is intended that this function be called
* on a warm reset of the P2020 device.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysGpioReset (void)
{
    GPIO_KEYDECLARE;

    GPIO_INTLOCK (GPIO_KEYDECLARE);

    GPIO_REG_WRITE(GPDIR, 0x00);
    GPIO_REG_WRITE(GPODR, 0x00);
    GPIO_REG_WRITE(GPDAT, 0x00);
    GPIO_REG_WRITE(GPIMR, 0x00);
    GPIO_REG_WRITE(GPIER, 0xffff0000);
    GPIO_REG_WRITE(GPICR, 0x00);

    GPIO_INTUNLOCK (GPIO_KEYDECLARE);

    return;
}

/*******************************************************************************
*
* sysGpioDirGet - get the direction of a GPIO pin number
*
* This routine returns the direction, 0x00 or 0x00, for
* a specified GPIO <lineNo> to the location specified in the <gpioDir>
* parameter.
*
* RETURNS:
* 0 when the direction is returned, else -1 if the specified pin
* number is out of range.
*
* ERRNO
*/

int sysGpioDirGet(UINT8   lineNo,UINT8 * gpioDir)
{
    VUINT32 gpioDirReg;
    UINT32  shifter;

    if ((lineNo > GPIO_PIN_MAX) || ( gpioDir == NULL))
        return (-1);

    GPIO_REG_READ(GPDIR, gpioDirReg);

    shifter = (0x80000000 >> lineNo);

    if (shifter & gpioDirReg) {
        *gpioDir = 0x01;
    } else {
        *gpioDir = 0x00;
    }

    return (0);
}


void gpio_set_fpga(void)
{
    int i=0;
    for(i=0;i<200;i++)
        printf("a\b");
    unsigned int ret=0;
    uint32_t spmode0 = 0x2F170008;


    SPI_REG_WRITE_32 (SPMODE0, spmode0 & ~ESPI_CSMODE_REV_MSB_FIRST);
    sysGpioLineConfig(11,GPIO_IN);
    sysGpioLineConfig(12,GPIO_IN);
    sysGpioLineConfig(13,GPIO_OUT);
    sysGpioLineSet(13,GPIO_HIGH);

redo1:
    ret = 0;
    sysGpioLineGet(&ret,12);
    sysGpioLineSet(13,GPIO_LOW);
    for(i=0;i<20;i++)
        printf(" \b");

    ret = 0;
    sysGpioLineGet(&ret,12);
    if (ret == GPIO_HIGH)
        goto redo1;
    
    ret = 0;
    sysGpioLineSet(13,GPIO_HIGH);
    for(i=0;i<20;i++)
        printf(" \b");
    sysGpioLineGet(&ret,12);
    if (ret != GPIO_HIGH)
        goto redo1;

    ret=0;
    sysGpioLineGet(&ret,11);
}

int gpio_check_fpga(void)
{
    udelay(1000000);
    unsigned int ret1=0,ret2=0;
    int i=0;
    int x=0;

redo2:
    ret1=0;
    ret2=0;
    sysGpioLineGet(&ret1,11);
    sysGpioLineGet(&ret2,12);

    for(i=0;i<20;i++)
        printf(" \b");
    if(ret1 != GPIO_HIGH || ret2 != GPIO_HIGH){
        x++;
        if (x>3000)
            return -1;
        goto redo2;
    }
    return 0;
}

int32_t sysSpiWriteData(uint32_t cs,uint32_t cmdlen,uint32_t datalen,
        uint8_t *cmd,uint8_t * data,uint32_t mode)
{
    uint32_t command = 0;
    uint32_t dataCount = 0;
    uint32_t tx1;
    uint32_t timeout=16;

    if (data != NULL) {
        dataCount = (datalen / 4 + ((datalen % 4 != 0) ? 1 : 0));
    }

    udelay(20);
    command = SPCOM_CS (cs) | (cmdlen + datalen - 1) ;

    /*
     * if TRANSONLY=0, RxSKIP=0, the eSPI changes to full duplex mode
     * if TRANSONLY=1, RxSKIP is ignored, no reception, useful for write
     * transcations.
     * RxSKIP: Num of ch skipped for reception from frame start, Non-zero
     * values of RxSKIP force the eSPI to half-duplex mode.
     */

    command |= SPCOM_TO (mode);

    /* only write command, so skip the cmdlen */

    if (data == NULL) {
        command |= SPCOM_RxSKIP (cmdlen);
    }
    SPI_REG_WRITE_32 (SPCOM, command);

    while ( timeout-- ) {
        tx1=SPI_REG_READ_32(SPIE);
        if ((tx1&0x3f0000)!=0){
            SPI_REG_WRITE_32 (SPITF, *(uint32_t *) data);
            data += 4;
            /*
             * delay for one IO WRTIE data cycle.
             * This delay is the experienced value. To get high performance,
             * it could be optimized by reducing the delay.
             */
            udelay(3);
            EIEIO_SYNC;    /* eieio, sync */
            dataCount--;
        }
        if(dataCount==0)
            break;
    }
    return OK;
}

int32_t cycloneFpgaTx(uint8_t  chipSelect,uint32_t startAddr,uint32_t  len,uint8_t * buf)
{
    uint32_t cmd32; 
    uint32_t dataLen = 0;
    uint32_t offset = 0;
    uint32_t x = len/30;
    uint32_t tx1=0;
    uint32_t timeout=0x30000;

    /* Probe the Spansion device */ 
    while (timeout --) {
        tx1=SPI_REG_READ_32(SPIE);
        if ((tx1&0x3f0000)!=0){
            /* Compute the number of data ready to write for one write cycle */
            dataLen = min (len - offset, 32);
            cmd32 = (offset + startAddr) & 0xffffff;
            if (sysSpiWriteData (chipSelect, 0, dataLen, (uint8_t*)&cmd32,buf + offset, TRANSONLY) != OK)
                return ERROR;
            if (offset%x==0)
                printf(".");
            offset += dataLen;
        }
        else{
            udelay(1);
        }
        if (offset >= len)
            break;
    }
    return OK;
}                                                                          


uint32_t fflag =0;
void spi_write_fpga(void)
{
    printf("write fpga\n");
    uint32_t len=0;
    uint32_t *flag_addr=(uint32_t*)FPGA_FLAG_ADDR;

    if (fflag == 0){
        if(*flag_addr == FLAG_A){
            len = *(uint32_t *)FPGA_A_LEN_ADDR;
            fflag = 1;
        }
        else {
            len = *(uint32_t *)FPGA_B_LEN_ADDR;
            fflag = 2;
        }
    }
    else{
        if (fflag == 1){
            len = *(uint32_t *)FPGA_B_LEN_ADDR;
            fflag = 3;
        }
        else {
            len = *(uint32_t *)FPGA_A_LEN_ADDR;
            fflag = 3;
        }
    }

    printf("len %d\n",len);
    if (len <=0 || len >10*1024*1024){
        printf("%d len error\n",fflag);
        return ;
    }
    memcpy((uint8_t *)0x1e00000,(uint8_t *)FPGA_A_DATA_ADDR,len);
	if (cycloneFpgaTx(0,0,len,(uint8_t *)0x1e00000)!=OK){
        printf("fpga write fail\n");
    }
    else{
        printf("fpga write done\n");
    }
}
