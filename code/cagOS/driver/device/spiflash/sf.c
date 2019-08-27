/*
 *------------------------------------------------------------
 * Project: cagOS
 * Goal:
 * Copyright (c) 2013, 2014 CAG Systems, Inc.
 *------------------------------------------------------------
 *
 *    @@@@     @@@      @@@@@       ####      ######
 *  @@       @@  @@    @@         ##    ##   ##
 * @@       @@    @@  @@   @@@@  ##      ## ######
 * @@      @@@@@@@@@@ @@    @@    ##    ##      ##
 *  @@@@@ @@@      @@@  @@@@@@      ####  #######
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable CAG license agreement.
 *
 * 1 tab == 4 spaces!
 *------------------------------------------------------------
 * modification history
 * --------------------
 * 2015.10.21, Ezio created
 *
 * DECRIPTION
 * fpga spi flash driver source file.
 *
 */

#include "sf.h"
#include <libc/string.h>

#define SEND_BUF    0x000000
#define RECV_BUF    0x000000

#define S_WR_TO 0x7000000
#define S_E_TO  0x70000000

#define sf_log(log)
int sf2_flash_errno = 0;
static UINT16 sf_get_stat(void)
{
    UINT16 stat;

    stat = fpga_read(SF_REG_STAT);
//    LOG_DEBUG("stat %x\n",stat);

    if (stat & 0x8000)
        return 0xe;  /* in progress*/

    stat &= 0xff;

    switch(stat) {
    case 0:
        break;
    case 1:
        break;
    default:
        //stat = 0xf;
        break;
    }
    return stat;
}

static void sf_set_addr(UINT32 addr)
{
    UINT16 hi,lo;
    hi = (addr>>16) & 0xff;
    lo = addr & 0xffff;
    fpga_write(SF_REG_ADDRH,hi);
    fpga_write(SF_REG_ADDRL,lo);
}
#if 0
static UINT32 sf_get_addr(void)
{
    UINT16 hi,lo;
    hi = fpga_read(SF_REG_ADDRH) & 0xff;
    lo = fpga_read(SF_REG_ADDRL);

    return (hi<<16)|lo;
}

static UINT32 sf_get_timeout(void)
{
    UINT16 hi,lo;
    hi = fpga_read(SF_REG_TOH);
    lo = fpga_read(SF_REG_TOL);

    return (hi<<16)|lo;
}
#endif

static void sf_set_timeout(UINT32 to)
{
    UINT16 hi,lo;
    hi = (to>>16);
    lo = to& 0xffff;
    fpga_write(SF_REG_TOH,hi);
    fpga_write(SF_REG_TOL,lo);
}

static INT32 sf_addr_ck(UINT32 addr,UINT32 cnt)
{
    if((addr&(SS_SIZE-1)) + cnt >= 0x800000) {
        sf_log(SF_PARA_ERR1);
        return -1;
    }

    switch(addr) {
    case 0 ... 0x7fffff:
        if (((addr&(SS_SIZE-1))+cnt)>=0x800000) {
            sf_log(SF_PARA_ERR2);
            return -1;
        } else
            return 0;
    case 0x800000 ... 0xffffff:
        if ((((addr-0x800000)&(SS_SIZE-1))+cnt)>=0x800000) {
            sf_log(SF_PARA_ERR3);
            return -3;
        } else
            return 1;
    default:
        sf_log(SF_PARA_ERR4);
        return -2;
    }
}

INT32 spiflash_read(UINT32 addr , UINT8 *buf , UINT32 cnt)
{
    UINT16 stat;
    UINT16 cmd ;
    UINT32 to= 0x10000;
    UINT32 to2 = S_WR_TO;
    LOG_DEBUG("%s read at 0x%x cnt %d\n",__FUNCTION__,addr,cnt);

    //check parameter
    if ((cmd = sf_addr_ck(addr,cnt))<0) {
        sf_log(SF_PARA_ERR5);
        return -1;
    }

#if 0
    if(sf_get_stat() == 0xe)
        return -3;
#else
    while((sf_get_stat() == 0xe) && to2--) {
    }
#endif
//    printf("esr %d to %x\n",sf_get_stat(),to2);

    //send command
    sf_set_timeout(to);
    sf_set_addr(addr-cmd*0x800000);
    fpga_write(SF_REG_LEN,cnt);

    if (cmd == 1)
        cmd = SF_CMD_CS1|SF_CMD_RD;
    else
        cmd = SF_CMD_RD;

    fpga_write(SF_REG_CMD , cmd);

    //wait for complicaton

    //while((stat = sf_get_stat()) == 0xe)// && to--)
    //while((stat = sf_get_stat()) == 0xe || (stat = sf_get_stat()) == 0)// && to--)
    //    ;

    to2 = S_WR_TO;

    while(to2--) {
        stat= sf_get_stat();
        if (stat != 0xe && stat != 0)
            break;
    }

    fpga_write(SF_REG_STAT,0xffff);

    LOG_DEBUG("stat1 %d\n",stat);

    if(to2 == 0 || stat !=1 ) {
        /*printf("sf r f %d %x\n",stat,addr);*/
        sf_log(SF_READFAIL_ERR);
        return -4;
    }

    //get data
#if 0
    if (pcie_recv(2,RECV_BUF,0,cnt)<0)
        return -2;
#else
    memcpy(buf,
            (unsigned char *)0xa0200000,
            cnt);
#endif
#if 0
    for(int j = 0; j<20; j++)
        LOG_DEBUG("%x ",buf[j]);
    LOG_DEBUG("\n");
#endif

    return cnt;
}

INT32 spiflash_write(UINT32 addr , UINT8 *buf , UINT32 cnt)
{
    UINT16 stat;
    UINT16 cmd;
    UINT32 to= 0x10000;
    UINT32 to2 = S_WR_TO;

    LOG_DEBUG("%s write at 0x%x cnt %d\n",__FUNCTION__,addr,cnt);
#if 0
    for(int j = 0; j<20; j++)
        LOG_DEBUG("%x ",buf[j]);
    LOG_DEBUG("\n");
#endif
    //check parameter
    if ((cmd=sf_addr_ck(addr,cnt))<0) {
        sf_log(SF_PARA_ERR6);
        return -1;
    }

#if 0
    if(sf_get_stat() == 0xe)
        return -3;
#else
    while((sf_get_stat() == 0xe) && to2 --) {
    }
#endif
//    printf("esw %d to %x\n",sf_get_stat(),to2);

    //send data
#if 0
    if (pcie_send(2,0,SEND_BUF,cnt)<0)
        return -2;
#else
    memcpy((unsigned char *)0xa0200000,
            buf,
            cnt);
#endif

    //send command
    sf_set_timeout(to);
    sf_set_addr(addr-cmd*0x800000);
    fpga_write(SF_REG_LEN,cnt);

    if (cmd == 1)
        cmd = SF_CMD_CS1|SF_CMD_WR;
    else
        cmd = SF_CMD_WR;

    fpga_write(SF_REG_CMD , cmd);

    //wait for complication
    //while((stat = sf_get_stat()) == 0xe)// && to--)
    // while((stat = sf_get_stat()) == 0xe || (stat = sf_get_stat()) == 0)// && to--)

    to2 = S_WR_TO;
    while(to2--) {
        stat= sf_get_stat();
        if (stat != 0xe && stat != 0)
            break;
    }


    fpga_write(SF_REG_STAT,0xffff);

    LOG_DEBUG("stat2 %d\n",stat);

    if(to2 == 0 || stat != 1) {
//        printf("sf w f %d %x\n",stat,addr);
        sf_log(SF_WRITEFAIL_ERR);
        return -4;
    }

    return cnt;

}


INT32 spiflash_erase(INT32 no , INT32 num)
{
    UINT16 stat;
    UINT16 cmd;
    UINT32 to= 0x10000;
    UINT32 to2 = S_E_TO;

    LOG_DEBUG("%s ",__FUNCTION__);

    //check parameter
    if ((cmd = sf_addr_ck(no*SS_SIZE,num*SS_SIZE))<0) {
        LOG_DEBUG("addrck fail %d %d\n",no,num);
        sf_log(SF_PARA_ERR7);
        return -1;
    }

#if 0
    if(sf_get_stat() == 0xe) {
        LOG_DEBUG("stat error \n");
        return -3;
    }
#else
    while((sf_get_stat() == 0xe) && to2--) {
    }
#endif
//    printf("es %d to %x\n",sf_get_stat(),to2);

    LOG_DEBUG("erase addr 0x%x , cmd %d\n",no*SS_SIZE-cmd*0x800000,cmd);

    //send command
    sf_set_timeout(to);
    sf_set_addr(no*SS_SIZE-cmd*0x800000);

    fpga_write(SF_REG_LEN,num);

    if (cmd == 1)
        cmd = SF_CMD_CS1|SF_CMD_ER;
    else
        cmd = SF_CMD_ER;

    fpga_write(SF_REG_CMD , cmd);

    //wait for complicaton
    //while((stat = sf_get_stat()) == 0xe || (stat = sf_get_stat()) == 0)// && to--)
    //    ;
    to2=S_E_TO;
    while(to2--) {
        stat= sf_get_stat();
        if (stat != 0xe && stat != 0)
            break;
    }

    fpga_write(SF_REG_STAT,0xffff);

    LOG_DEBUG("stat3 %d\n",stat);

    if(to2 == 0 || stat !=1 ) {
        //printf("stat fail 2 %x %d-%d\n",stat,no,num);
        sf_log(SF_ERASEFAIL_ERR);
        return -4;
    }

    return 0;

}

#if 0
//temprory not used
INT32 spiflash_id()
{
    UINT16 stat;
    UINT16 cmd;
    UINT32 to= S_WR_TO;
    UINT32 to2 = S_WR_TO;

    UINT8 *buf=(UINT8*)0x1fa00000;

    if(sf_get_stat() == 0xe)
        return -3;

    sf_set_timeout(to);

    cmd = SF_CMD_ID|SF_CMD_CS0;
    fpga_write(SF_REG_CMD , cmd);

    int i=0;

    //while(((stat = sf_get_stat()) == 0xe) && to--)
    //    LOG_DEBUG("stag %d\n",i++);

    while(1) {
        stat= sf_get_stat();
        if (stat != 0xe && stat != 0)
            break;
    }

    fpga_write(SF_REG_STAT,0xffff);

    if(to==0 || stat !=1 ) {
        LOG_DEBUG("-4 stat %x\n",stat);
        return -4;
    }

#if 0
    if( pcie_recv(2,RECV_BUF,0,20)<20)
        return -2;
#else
    memcpy(0x1fa00000,0xa0200000,20);
#endif

    LOG_DEBUG("fpga spi flash id :\n");
    for(int i =0; i<20; i++)
        LOG_DEBUG("%x",buf[i]);

    LOG_DEBUG("\n");
    return 0;
}
#endif
