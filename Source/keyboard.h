void KeyBoardInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	PAout(0)=1;
	PCout(14)=1;
	PCout(15)=1;
}

//返回1-9,10是兴号键,11是0,12是警号键
vu8 Key_Scan(void)

{
	vu8 i,KeyVal=255;

    PAout(4)=1;
    PAout(3)=1;
    PAout(2)=1;
    PAout(1)=1;

	for(i=0;i<4;i++)//扫qqqq描行
	{  	
		if(i==0)
		{
			PAout(4)=0;
			PAout(3)=1;
			PAout(2)=1;
			PAout(1)=1;
		}
		if(i==1)
		{
			PAout(4)=1;
			PAout(3)=0;
			PAout(2)=1;
			PAout(1)=1;
		}
		if(i==2)
		{
			PAout(4)=1;
			PAout(3)=1;
			PAout(2)=0;
			PAout(1)=1;
		}	  
		if(i==3)
		{
			PAout(4)=1;
			PAout(3)=1;
			PAout(2)=1;
			PAout(1)=0;
		}

		
		
		if(PCin(14)==0)//最右边的键
		{		 	
			delay_ms(15);
			if(PCin(14)==0)//最右边的键
			 	KeyVal=3+i*3;	
			while(PCin(14)==0);
            break;
		}
		else if(PCin(15)==0)//中间的键
		{
			delay_ms(15);
			if(PCin(15)==0)//最右边的键
				KeyVal=2+i*3;
			while(PCin(15)==0);
            break;
		}
		else if(PAin(0)==0)//最左边的键
		{
			delay_ms(15);
			if(PAin(0)==0)//最右边的键			
				KeyVal=1+i*3;
			while(PAin(0)==0);
            break;
		
		}

	}
    return KeyVal;

}

