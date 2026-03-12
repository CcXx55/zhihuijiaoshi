#include <stdlib.h>
vu8 jkg[6];
//#define _lanya_
//吧蓝牙数据转,然后把之前串口中断屏蔽,用下面这个
//因为蓝牙没发送时会一直发送0
void lanya(void)
{

    vu8 i,j=0,k=0,bb[7];
		
		if( Uart_RecOk1)
		{
			 Uart_RecOk1=0;
				RecCnt1++;//多加1

				for(i=0;i<RecCnt1;i++)
				{
						
					 if((Uart_Rx1[i]==',')||((i+1)==RecCnt1))
					 {
							
							bb[k]=0;
							jkg[j++] =atof((const char *)&bb[0]);
							k=0;
					 }
					 else
					 {
							if(k<6)
								bb[k++]=Uart_Rx1[i];
					 }
				}
				//会自动调用宏定义,从而接收中断不会接收到0这个数据
				if((jkg[0]==0x66)&&(jkg[5]==0x88))
				{
						sys[0]=jkg[1];
						sys[1]=jkg[2];
						sys[2]=jkg[3];
						sys[3]=jkg[4];
						SysSave();
						
				}
		}
}

