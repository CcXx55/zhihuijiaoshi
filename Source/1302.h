vu8 hour=0,min=0,sec=0,year=0,month=0,date=0;

void ds1302(vu8 uart)
{
    vu8 *uart1302recok,*uart1302;

vu8 aa[6];
    if (uart == 1)
    {
        uart1302recok = &Uart_RecOk1;
        uart1302= &Uart_Rx1[0];
    }
    else
    {

        uart1302recok = &Uart_RecOk2;
        uart1302 = &Uart_Rx2[0];
    }

    if (uart1302recok) // 收到数据,数据会一直发送,定时器不需要走时",
    {
        uart1302recok = 0;                  // 清除标志
        if ((uart1302[0] == 'd') && (uart1302[1] == 'a')) // 在0x66与0x88直接的数据就是真实数据
        {
          aa[0]=(uart1302[7]-48)*10+(uart1302[8]-48);
          aa[1]=(uart1302[10]-48)*10+(uart1302[11]-48);
          aa[2]=(uart1302[13]-48)*10+(uart1302[14]-48);
           aa[3]=(uart1302[21]-48)*10+(uart1302[22]-48);

           aa[4]=(uart1302[24]-48)*10+(uart1302[25]-48);
           aa[5]=(uart1302[27]-48)*10+(uart1302[28]-48);

if((aa[0]<100)&&(aa[1]<13)&&(aa[2]<32)&&(aa[3]<24)&&(aa[4]<60)&&(aa[5]<60))
{
            year=aa[0];
            month=aa[1];
            date=aa[2];
            hour=aa[3];

            min=aa[4];
            sec=aa[5];
}
#if 1
            LCDW_Com(0x80);
 LCDW_Dat('2');
 LCDW_Dat('0');
            LCDW_Dat(year/10+48);
            LCDW_Dat(year%10+48);
            LCDW_Dat('-');
            LCDW_Dat(month/10+48);
            LCDW_Dat(month%10+48);
            LCDW_Dat('-');
            LCDW_Dat(date/10+48);
            LCDW_Dat(date%10+48);

                LCDW_Dat(' ');
            LCDW_Dat(hour/10+48);
            LCDW_Dat(hour%10+48);
            if(sec%2)
                LCDW_Dat(':');
            else
                LCDW_Dat(' ');
            LCDW_Dat(min/10+48);
            LCDW_Dat(min%10+48);
#endif
        }
    }
}

