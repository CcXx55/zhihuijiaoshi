
vu8 xinlv=0,sop2=0;

void max30100Deal(vu8 uart)//100ms调用这个函数
{
   
		vu8 *a30100,*b30100; 
		if(uart==1)
		{
			a30100=&Uart_RecOk1;
			b30100=&Uart_Rx1[0];
		}
		else
		{
			a30100=&Uart_RecOk2;
			b30100=&Uart_Rx2[0];
		}		


		if(*a30100)
		{
				*a30100=0;
				if((b30100[0]==0X66) &&(b30100[3]==0X88))//命令设置成功,返回OK
				{

						xinlv=b30100[1];
if(xinlv>99)
xinlv=99;

						sop2=b30100[2];
if(sop2>99)
sop2=99;
					
				}			
				
			
	 }

}
