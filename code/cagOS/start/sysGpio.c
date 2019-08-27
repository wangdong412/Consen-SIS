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

/* defines */

/* GPIO Registers */
#include <config.h>
#include <libc/gpio.h>

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

int sysGpioLineConfig
(
    UINT8  lineNo,
    UINT32 style
)
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

int sysGpioLineGet
(
    UINT32 * gpioData,
    UINT8    lineNo
)
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

int sysGpioLineSet
(
    UINT8   lineNo,
    UINT8   value
)
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

int sysGpioIntStatusGet
(
    UINT8  lineNo,
    char * status
)
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

int sysGpioIntStatusClear
(
    UINT8 lineNo
)
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

int sysGpioDirGet
(
    UINT8   lineNo,
    UINT8 * gpioDir
)
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

