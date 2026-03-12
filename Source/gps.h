

/*
	---保存站点(最多11个)
	   GQQQ 1 2455.800611838.6877

	---到站
		读取oldl80,=99,没到站,为0-9,则到具体的站

	---单片机调用
						for(i=0;i<8;i++)
							sendchar(1,weidu[i]);
						sendchar(1,',');

						for(i=0;i<9;i++)
							sendchar(1,jingdu[i]);
*/


vu8 weidu[10]= {"________\0"},jingdu[10]= {"_________\0"};
vu8 oldl80=99,speed=0;


//#define l80disflag//要不要吧经纬度显示在液晶


void l80Deal(vu8 uart)//读取oldl80,=99,没到站,为0-9,则到具体的站
{
    vu8 i,*bl80,*al80,cl80;
    if(uart==1)
    {
        al80=&Uart_RecOk1;
        bl80=&Uart_Rx1[0];
        cl80=RecCnt1;
    }
    else
    {

        al80=&Uart_RecOk2;
        bl80=&Uart_Rx2[0];
        cl80=RecCnt2;

    }
    if(*al80)
    {
        *al80=0;
#if 0

        year=bl80[1];
        month=bl80[2];
        date=bl80[3];
        hour=bl80[4];
        min=bl80[5];
        sec=bl80[6];
        speed=bl80[7];
#endif
        if((bl80[0]==0x66) &&(bl80[cl80-1]==0x88))
        {

            //定义日期显示位置
#if 0

            LCDW_Com(0x80+0x40);
            LCDW_Dat('2');
            LCDW_Dat('0');
            LCDW_Dat(bl80[1]/10+48);
            LCDW_Dat(bl80[1]%10+48);
            LCDW_Dat('-');
            LCDW_Dat(bl80[2]/10+48);
            LCDW_Dat(bl80[2]%10+48);
            LCDW_Dat('-');
            LCDW_Dat(bl80[3]/10+48);
            LCDW_Dat(bl80[3]%10+48);
#endif
            //定义时间显示位置
#if 0
            LCDW_Com(0x80+0x40);
            i=bl80[4];

            LCDW_Dat(i/10+48);
            LCDW_Dat(i%10+48);
            LCDW_Dat(':');
            LCDW_Dat(bl80[5]/10+48);
            LCDW_Dat(bl80[5]%10+48);
            LCDW_Dat(':');
            LCDW_Dat(bl80[6]/10+48);
            LCDW_Dat(bl80[6]%10+48);
#endif
            //定义速度显示位置
//						LCDW_Com(0x80+0x40);
//						LCDW_Dat(bl80[7]/10+48);
//						LCDW_Dat(bl80[7]%10+48);

            //定义纬度显示位置
#ifdef l80disflag
            LCDW_Com(0x80);
            LCDW_Dat('W');
#endif
            for(i=0; i<8; i++)
            {
                weidu[i]=bl80[8+i];
#ifdef l80disflag
                LCDW_Dat(bl80[8+i]);
#endif
            }
            //定义经度显示位置
#ifdef l80disflag
            LCDW_Com(0x80+0x40);
            LCDW_Dat('J');
#endif
            for(i=0; i<9; i++)
            {
                jingdu[i]=bl80[8+8+i];
#ifdef l80disflag
                LCDW_Dat(bl80[8+8+i]);
#endif
            }

            oldl80=(bl80[8+8+9])*10+(bl80[8+8+10]);


        }

    }


}
