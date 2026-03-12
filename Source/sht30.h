
vu16 sht30Temp,sht30Damp;//数据是精确到小数点

void sht30Deal(vu8 uart)
{
   
		vu8 *asht30,*bsht30; 
		if(uart==1)
		{
			asht30=&Uart_RecOk1;
			bsht30=&Uart_Rx1[0];
		}
		else
		{
			asht30=&Uart_RecOk2;
			bsht30=&Uart_Rx2[0];
		}		

		
		if(*asht30)
		{
				*asht30=0;
				if((bsht30[0]=='S') &&(bsht30[9]=='E'))//命令设置成功,返回OK
				{
						sht30Temp=(bsht30[1]-48)*10+(bsht30[2]-48);
						sht30Damp=(bsht30[5]-48)*10+(bsht30[6]-48);
				}
		}
}

