vu16 juli=0;//单位mm
vu8 cntsr04=0;
#define sr04Deal chaoshengboDeal
vu8 chaoshengboDeal(vu8 uart)//100ms调用这个函数
{
 
		vu8 *asr04,*bsr04; 
		cntsr04++;
		if(cntsr04>=5)
		{
			cntsr04=0;
			sendstr(uart,"sr04see");
		}
		if(uart==1)
		{
			asr04=&Uart_RecOk1;
			bsr04=&Uart_Rx1[0];
		}
		else
		{
			asr04=&Uart_RecOk2;
			bsr04=&Uart_Rx2[0];
		}		

		
		if(*asr04)
		{
				*asr04=0;
				if((bsr04[0]=='J') &&(bsr04[1]=='='))
				{

						//定义日期显示位置(2000-01-24)
					//	LCDW_Com(0x80+0x40);
				//		for(i=0;i<4;i++)
					//		LCDW_Dat(bsr04[2+i]);
					
						//单位mm	
						juli=(bsr04[2]-48)*1000+(bsr04[3]-48)*100+(bsr04[4]-48)*10+(bsr04[6]-48);
						juli/=10;
						if(juli<10)
							return 1;
					
												
			
				}
			
	  }
		return 0;
}

