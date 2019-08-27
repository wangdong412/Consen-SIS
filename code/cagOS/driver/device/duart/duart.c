#include "duart.h"


struct duart *sio[2];

INT32 check_no(UINT8 no)
{
    INT32 ret=0;
    if (no !=1 && no != 2)
        ret = -1;
    return ret;
}
void init_uart(UINT8 no)
{
    UINT32 base;
    UINT32 uartClk = sysUartClkRateGet();

    if(no==1){
        base = DUART1_BASE;
        sio[no]=(struct duart *)DUART1_BASE; 
    }
    else if (no==2){
        base = DUART2_BASE;
        sio[no]=(struct duart *)DUART2_BASE; 
    }
    else{
        base = 0;
    }


    /*fifo mode*/
    if(base !=0){
        printf("base %x\n",base);

        outb(&sio[no]->ier,0);

        outb(&sio[no]->lcr,CHAR_LEN_8|ONE_STOP|LCR_DLAB);
        outb(&sio[no]->dlb,BAUD_LO(uartClk, _CAG_CONFIG_SIO_BAUDRATE));
        outb(&sio[no]->dmb,BAUD_HI(uartClk, _CAG_CONFIG_SIO_BAUDRATE));

        outb(&sio[no]->lcr,CHAR_LEN_8|ONE_STOP);
        outb(&sio[no]->fcr,FCR_RXCLR| FCR_TXCLR| RTL_1|FCR_DMS);
        outb(&sio[no]->fcr,FCR_RXCLR| FCR_TXCLR| FCR_EN | RTL_1|FCR_DMS);

        printf("iir %x\n",inb(&sio[no]->iir));

    }
    printf("lsr %x\n",inb(&sio[no]->lsr));
}

INT16 uart_send(UINT8 no,UINT8 num, UINT8 *data)
{
    int i=0;
    if (check_no(no) ==0){
        //while((inb(&sio->dsr) & 0x2)!=0x0)
        if((inb(&sio[no]->dsr) & 0x2)!=0x0 && (inb(&sio[no]->lsr)&0x60)==0x60){
        }
        else{

            UINT32 t1 = getTblus();
            for(i=0;i<num&&i<16;i++){
               outb(&sio[no]->thr,*(data+i) );
            }
            UINT32 t2 = getTblus();
            printf("s %d\n",t2-t1);
        }
    }
    else {
        i=-1;
    }

    return i;
}

INT8 xxxy[1024]={0};
INT32 xxxi=0;


/*
 * 1. 使用 fifo 模式时，以fifo溢出为收到数据的标志，对端最少发送17bytes，控制器才能提示收到数据，并且最后一个个字节会被略去
 * 2. 如果以收到数据为标志，则要求收到第一个字节后等待1.4ms才能收完剩余15个字节
 * 3. 向控制发包间隔需要大于1.5ms，否则fifo会溢出
 */
INT16 uart_recv(UINT8 no,UINT8 num, UINT8 *data)
{
    INT16 ret=0,i=0;

    if (check_no(no) ==0) {
        if((inb(&sio[no]->dsr)&0x1)==0x1 || (inb(&sio[no]->lsr)&0x2) == 0x0 ){// && (inb(&sio->lsr)&0x1)==0){
        }
        else{
            if ((inb(&sio[no]->lsr)&0x8e) != 0){
                printf("err\n");
                return i;
            }
    //        udelay(1400);
            UINT32 t1 = getTblus();
            for(i=0;i<num+1&&i<17;i++){
                *(data+i)=inb(&sio[no]->rbr);
    //            xxxy[xxxi++]=inb(&sio->rbr);
    //            printf("R%x %d\n",*(data+i),i);
            }
            outb(&sio[no]->fcr,FCR_RXCLR|FCR_EN|RTL_1|FCR_DMS);
            uart_clear_fifo(2);
            UINT32 t2 = getTblus();
            printf("r %d\n",t2-t1);
        }
    //    printf("rlsr %x\n",inb(&sio->lsr));

    }
    else {
        i=-1;
    }
    return i;
}

void uart_clear_fifo(int no)
{
    if (check_no(no) ==0) {
        outb(&sio[no]->fcr,0);
        outb(&sio[no]->fcr,FCR_RXCLR| FCR_TXCLR| FCR_EN | RTL_1|FCR_DMS);
    }
}

