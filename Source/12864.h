

/***********************************************************************************************
函数名:	 往1602写命令
输入参数: com:命令值
输出:无
说明:无
***********************************************************************************************/
void L12864_Com(u8 com)
{
	vu8 n=0,s=0;
	vu16 date=(GPIO_ReadOutputData(GPIOB)&0x1300)+com;
    PIN_LCD_RS=0;
	delay_ms(1);
	if((com&0x08)==0x08)
	{
		n=1;
		PBout(10)=1;
	}
	else
		PBout(10)=0;
	if((com&0x10)==0x10)
	{
		s=1;
		PBout(11)=1;
	}
	else
		PBout(11)=0;

	GPIO_Write(GPIOB,date+(n<<10)+(s<<11));

	PIN_LCD_EN=1;

	PIN_LCD_EN=0;
	delay_ms(1);
}

/***********************************************************************************************
函数名:	 往1602写数据
输入参数:dat:数据
输出:无
说明:无
***********************************************************************************************/
void L12864_Dat(u8 dat)
{
	vu8 n=0,s=0;
	vu16 date=(GPIO_ReadOutputData(GPIOB)&0x1300)+dat;
	PIN_LCD_RS=1;
	delay_ms(1);
	if((dat&0x08)==0x08)
	{
		n=1;
		PBout(10)=1;
	}
	else
		PBout(10)=0;
	if((dat&0x10)==0x10)
	{
		s=1;
		PBout(11)=1;
	}
	else
		PBout(11)=0;

	GPIO_Write(GPIOB,date+(n<<10)+(s<<11));

	PIN_LCD_EN=1;

	PIN_LCD_EN=0;
	delay_ms(1);
}


/***********************************************************************************************
函数名:在指定的位置显示温度
输入参数:
 1.Line:行,取值0,1
 2.Row:列,取值0~15
 3.Dat:要显示的数据
输出:无
说明:无
***********************************************************************************************/
void L12864_DisStr(u8 Line,u8 Row,u8 *Dat)
{
	if(Line==0)
		L12864_Com(0x80+Row);//指定显示位置
	else if(Line==1)
		L12864_Com(0x90+Row);//指定显示位置
	else if(Line==2)
		L12864_Com(0x88+Row);//指定显示位置
	else if(Line==3)
		L12864_Com(0x98+Row);//指定显示位置
	
	while(*Dat!='\0')
		L12864_Dat(*Dat++);
			
}

//1602初始化
void L12864_Init(void)  
{
	vu16 i;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	//引脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	PIN_LED_Fac=1;
	PIN_LED_Warn=1;

	Beep=0;

	L12864_Com(0x30);//使用基本指令集
	L12864_Com(0x01);//清屏
	L12864_Com(0x0c);//打开显示
	L12864_Com(0x06);//游标右移，AC加1

	L12864_Com(0x01);

	


    
}
vu8 FlagDisTime=0;

void DisTime(void)
{

    static vu8 SecOld=255;
    if(FlagDisTime==0)
        return;
	RTC_Get();
	if(SecOld!=calendar.second)
	{
		SecOld=calendar.second;	
		L12864_Com(0x80);
		L12864_Dat('2');
		L12864_Dat('0');
		L12864_Dat((calendar.w_year-2000)/10+48);
		L12864_Dat((calendar.w_year-2000)%10+48);
		L12864_Dat('-');
		L12864_Dat(calendar.w_month/10+48);
		L12864_Dat(calendar.w_month%10+48);		
		L12864_Dat('-');
		L12864_Dat(calendar.w_date/10+48);
		L12864_Dat(calendar.w_date%10+48);
		
       	
        if(calendar.week==1)
              L12864_DisStr(0,5,"星期一");	   
        if(calendar.week==2)
              L12864_DisStr(0,5,"星期二");	
        if(calendar.week==3)
              L12864_DisStr(0,5,"星期三");	   
        if(calendar.week==4)
              L12864_DisStr(0,5,"星期四");        
        if(calendar.week==5)
              L12864_DisStr(0,5,"星期五");	   
        if(calendar.week==6)
              L12864_DisStr(0,5,"星期六");        
        if(calendar.week==0)
              L12864_DisStr(0,5,"星期日");	   
      
		L12864_Com(0x90+2);
		L12864_Dat(calendar.hour/10+48);
		L12864_Dat(calendar.hour%10+48);
		L12864_Dat(':');
		L12864_Dat(calendar.minute/10+48);
		L12864_Dat(calendar.minute%10+48);		
		L12864_Dat(':');
		L12864_Dat(calendar.second/10+48);
		L12864_Dat(calendar.second%10+48);
			
	}
}
//新起一页,KeyA,KeyU为加减键,KeyS为移位键,
void SetTime(void)
{
	//修改时间
	
	vu8 bits=0,SysPage,i;
	if(PAin(5)==0)
	{
        while(PAin(5)==0);//等待松开
        FlagDisTime=0;
		L12864_Com(0X01);
		L12864_DisStr(0,2,"修改时间");		
		L12864_DisStr(1,0,"20  年  月  号");
		L12864_DisStr(2,0,"星期: ");
		L12864_DisStr(3,0,"  点  分  秒"); 
		SysPage=1;

		while(1)
		{
			if(SysPage)
			{
                RTC_Get();
				L12864_Com(0X0c);
				SysPage=0;
				L12864_Com(0X91);
				L12864_Dat((calendar.w_year-2000)/10+48);
				L12864_Dat((calendar.w_year-2000)%10+48);
				
				L12864_Com(0X93);
				L12864_Dat(calendar.w_month/10+48);
				L12864_Dat(calendar.w_month%10+48);
				
				L12864_Com(0X95);	
				L12864_Dat(calendar.w_date/10+48);
				L12864_Dat(calendar.w_date%10+48);
				
                if(calendar.week==1)
                      L12864_DisStr(2,0,"星期一");	   
                if(calendar.week==2)
                      L12864_DisStr(2,0,"星期二");	
                if(calendar.week==3)
                      L12864_DisStr(2,0,"星期三");	   
                if(calendar.week==4)
                      L12864_DisStr(2,0,"星期四");        
                if(calendar.week==5)
                      L12864_DisStr(2,0,"星期五");	   
                if(calendar.week==6)
                      L12864_DisStr(2,0,"星期六");        
                if(calendar.week==0)
                      L12864_DisStr(2,0,"星期日");	

				L12864_Com(0x98);
				L12864_Dat(calendar.hour/10+48);
				L12864_Dat(calendar.hour%10+48);
				L12864_Com(0x9A);
				L12864_Dat(calendar.minute/10+48);
				L12864_Dat(calendar.minute%10+48);		
				L12864_Com(0x9C);
				L12864_Dat(calendar.second/10+48);
				L12864_Dat(calendar.second%10+48);
		
			}
			
			if(bits==0)
				L12864_Com(0X91);	
			if(bits==1)
				L12864_Com(0X93);
			if(bits==2)
				L12864_Com(0X95);	
			if(bits==3)
				L12864_Com(0X8a);
			if(bits==4)
				L12864_Com(0x98);
			if(bits==5)
				L12864_Com(0x9A);
			if(bits==6)		
				L12864_Com(0x9C);
		
                L12864_Com(0x0f);
			if(Key==0)
			{
				delay_ms(10);
				if(Key==0)
				{
					bits++;
					if(bits>6)
						bits=0;
                    if(bits==3)
                        bits=4;
				}
				while(Key==0);
			}
			if(KeyA==0)
			{
				i=0;
				delay_ms(400);
				if(bits==0)
				{
					calendar.w_year++;
					if(calendar.w_year>2099)
						calendar.w_year=2000;
				}
				if(bits==1)
				{
					calendar.w_month++;
					if(calendar.w_month>12)
						calendar.w_month=1;
				}
				if(bits==2)
				{
					calendar.w_date++;
					if(calendar.w_date>31)
						calendar.w_date=1;
				}				
				if(bits==3)
				{
                    bits=4;
				}
				if(bits==4)
				{
					calendar.hour++;
					if(calendar.hour>23)
						calendar.hour=0;
				}
				if(bits==5)
				{
					calendar.minute++;
					if(calendar.minute>59)
						calendar.minute=0;
				}	
				if(bits==6)
				{
					calendar.second++;
					if(calendar.second>59)
						calendar.minute=0;
				}					
				RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.minute,calendar.second);
				SysPage=1;    
			}
			if(KeyU==0)
			{
				i=0;
				delay_ms(400);
				if(bits==0)
				{
					calendar.w_year--;
					if(calendar.w_year<2000)
						calendar.w_year=2099;
				}
				if(bits==1)
				{
					calendar.w_month--;
					if(calendar.w_month==0)
						calendar.w_month=12;
				}
				if(bits==2)
				{
					calendar.w_date--;
					if(calendar.w_date==0)
						calendar.w_date=31;
				}				
				if(bits==3)
				{
                   bits=4;
				}
				if(bits==4)
				{
					calendar.hour--;
					if(calendar.hour>23)
						calendar.hour=23;
				}
				if(bits==5)
				{
					calendar.minute--;
					if(calendar.minute>59)
						calendar.minute=59;
				}	
				if(bits==6)
				{
					calendar.second--;
					if(calendar.second>59)
						calendar.minute=59;
				}
				RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.minute,calendar.second);
				SysPage=1;
			}				
			
			if(PAin(5)==0)
			{
                RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.minute,calendar.second);
                break;
            }
		}
		L12864_Com(0X0c);
		while(PAin(5)==0);//等待松开
 		L12864_Com(0X01);  
        FlagDisTime=1;
	}
}


