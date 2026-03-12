
vu8 key=255;//0.51,则key=51
#define KeyBoard juzhenjianpanDeal
vu8 juzhenjianpanDeal(vu8 uart)//有按下函数返回1
{
   
		vu8 *akey,*bkey,i; 
		if(uart==1)
		{
			akey=&Uart_RecOk1;
			bkey=&Uart_Rx1[0];
		}
		else
		{
			akey=&Uart_RecOk2;
			bkey=&Uart_Rx2[0];
		}		

		
		if(*akey)
		{
				*akey=0;
				#if 1
						if((bkey[0]==0x66) &&(bkey[2]==0x88))//命令设置成功,返回OK
						{
								key=bkey[1];
								return 1;
						}
				#else//带密码功能
						//每按下按键,打印Ta,不跟其他if形成else if关系
						if(strstr((const char *)&bkey[0],"Ta"))
						{
								PBout(12)=0;
								delayms(100);
								PBout(12)=1;
						}
						if(strstr((const char *)&bkey[0],"Key Ok"))//密码正确
						{
								jdq=0;
								Beep=0;
								delayms(100);
								Beep=1;						
								LCD_DisStr(1,0,"Key OK  ");
								delayms(1000);
								jdq=1;
								LCD_DisStr(1,0,"        ");
						}
						if(strstr((const char *)&bkey[0],"Key Err"))
						{
								LCD_DisStr(1,0,"Key Err  ");
								Beep=0;
								delayms(500);//长叫代表错误
								Beep=1;	
								delayms(500);
								LCD_DisStr(1,0,"      ");
						}
						if(strstr((const char *)&bkey[0],"Key 3Err"))
						{
								LCD_DisStr(1,0,"Key 3Err");
								Beep=0;
								delayms(100);
								Beep=1;	
								delayms(100);
								Beep=0;
								delayms(100);
								Beep=1;	
								delayms(100);
								Beep=0;
								delayms(100);
								Beep=1;	
								delayms(100);	
								while(*akey==0);//收到Key Continue,则不再禁止输入密码
								*akey=0;
								LCD_DisStr(1,0,"        ");
						}				
				#endif
				for(i=0;i<30;i++)
				{
					bkey[i]=0;
				}
		}
		return 0;
}

