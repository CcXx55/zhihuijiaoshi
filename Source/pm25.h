
vu8 pm25;//0.51,则pm25=51
#define gp2yDeal pm25Deal
void pm25Deal(vu8 uart)
{
   
		vu8 *apm25,*bpm25; 
		if(uart==1)
		{
			apm25=&Uart_RecOk1;
			bpm25=&Uart_Rx1[0];
		}
		else
		{
			apm25=&Uart_RecOk2;
			bpm25=&Uart_Rx2[0];
		}		

		
		if(*apm25)
		{
				*apm25=0;
				if((bpm25[0]=='S') &&(bpm25[5]=='E'))//命令设置成功,返回OK
				{
						pm25=(bpm25[3]-48)*10+bpm25[4]-48;
				}
		}
}

