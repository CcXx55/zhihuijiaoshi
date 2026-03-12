
u32 dht11output_addr; // 代表用PA口,ODR代表输出
u32 dht11input_addr;  // 代表用PA口,IDR代表输入
vu8 dht11_bit;		  // 无论输入输出都是11口

vu8 U8T_data_H, U8T_data_L, U8RH_data_H, U8RH_data_L, U8checkdata;
vu8 U8T_data_H_temp, U8T_data_L_temp, U8RH_data_H_temp, U8RH_data_L_temp, U8checkdata_temp;
vu8 U8comdata;
vu8 U8FLAG, U8temp;
#define TempNow DHT11Temp
#define DampNow DHT11Damp
void Delay_10us(void)
{
	delay_us(15);
}

void COM(void)
{
	vu8 i;
	Delay_10us();
	BIT_ADDR(dht11output_addr, dht11_bit) = 1;
	for (i = 0; i < 8; i++)
	{

		U8FLAG = 2;

		while ((!BIT_ADDR(dht11input_addr, dht11_bit)) && U8FLAG++)
			;
		Delay_10us();
		Delay_10us();
		Delay_10us();
		U8temp = 0;

		if (BIT_ADDR(dht11input_addr, dht11_bit))
			U8temp = 1;
		Delay_10us();
		U8FLAG = 2;
		while ((BIT_ADDR(dht11input_addr, dht11_bit)) && U8FLAG++)
			;
		if (U8FLAG == 1)
			break;
		U8comdata <<= 1;
		U8comdata |= U8temp;
	}
}
// 写法DH11_GetTempDamp(GPIOA_ODR_Addr,GPIOA_IDR_Addr,11);//代表用PA11读温湿度,ODR代表输出,IDR输入,输入输出都11脚

void DH11_GetTempDamp(vu8 port, vu8 bit1)
{
	vu32 out1,  input;
	set(port, bit1, od);
	if (port == PA)
	{

		dht11output_addr = GPIOA_ODR_Addr; // 代表用PA口,ODR代表输出
		dht11input_addr = GPIOA_IDR_Addr;  // 代表用PA口,ODR代表输出
	}
	if (port == PB)
	{

		dht11output_addr = GPIOB_ODR_Addr; // 代表用PA口,ODR代表输出
		dht11input_addr = GPIOB_IDR_Addr;  // 代表用PA口,ODR代表输出
	}
	if (port == PC)
	{

		dht11output_addr = GPIOC_ODR_Addr; // 代表用PA口,ODR代表输出
		dht11input_addr = GPIOC_IDR_Addr;  // 代表用PA口,ODR代表输出
	}
	dht11_bit = bit1; // 比如bit=4,代表用PA4
	BIT_ADDR(dht11output_addr, dht11_bit) = 0;
	delay_ms(24);
	BIT_ADDR(dht11output_addr, dht11_bit) = 1;
	Delay_10us();
	Delay_10us();
	Delay_10us();
	Delay_10us();

	if (!BIT_ADDR(dht11input_addr, dht11_bit))
	{

		U8FLAG = 2;
		while ((!BIT_ADDR(dht11input_addr, dht11_bit)) && U8FLAG++)
			;
		U8FLAG = 2;
		while ((BIT_ADDR(dht11input_addr, dht11_bit)) && U8FLAG++)
			;
		COM();
		U8RH_data_H_temp = U8comdata;
		COM();
		U8RH_data_L_temp = U8comdata;
		COM();
		U8T_data_H_temp = U8comdata;
		COM();
		U8T_data_L_temp = U8comdata;
		COM();
		U8checkdata_temp = U8comdata;
		BIT_ADDR(dht11output_addr, dht11_bit) = 1;
		U8temp = (U8T_data_H_temp + U8T_data_L_temp + U8RH_data_H_temp + U8RH_data_L_temp);
		if (U8temp == U8checkdata_temp)
		{
			U8RH_data_H = U8RH_data_H_temp;
			U8RH_data_L = U8RH_data_L_temp;
			U8T_data_H = U8T_data_H_temp;
			U8T_data_L = U8T_data_L_temp;
			U8checkdata = U8checkdata_temp;
		}

		DHT11Temp = U8T_data_H;
		DHT11Damp = U8RH_data_H;
	}
}

#if 0

#define PIN_DHT11W1 PAout(11)
#define PIN_DHT11R1 PAin(11)
vu8 DHT11Temp1,DHT11Damp1;//温湿度,调用DH11_GetTempDamp(void)就能把得到的值放在这2个变量
vu8  U8T_data_H1,U8T_data_L1,U8RH_data_H1,U8RH_data_L1,U8checkdata1;
vu8  U8T_data_H_temp1,U8T_data_L_temp1,U8RH_data_H_temp1,U8RH_data_L_temp1,U8checkdata_temp1;
vu8  U8comdata1;
vu8  U8FLAG1,U8temp1;
#define TempNow1 DHT11Temp1
#define DampNow1 DHT11Damp1

void  COM1(void)
{     
	vu8 i; 
	Delay_10us(); 
	PIN_DHT11W1=1;	
	for(i=0;i<8;i++)	   
	{
	
		U8FLAG1=2;	

		while((!PIN_DHT11R1)&&U8FLAG1++);
		Delay_10us();
		Delay_10us();
		Delay_10us();
		U8temp1=0;

		if(PIN_DHT11R1)
			U8temp1=1;
		Delay_10us();
		U8FLAG1=2;
		while((PIN_DHT11R1)&&U8FLAG1++);	  
		if(U8FLAG1==1)
			break;	   	 
		U8comdata1<<=1;
		U8comdata1|=U8temp1; 
	}	   
}


void DH11_GetTempDamp1(void)
{

	PIN_DHT11W1=0;
	delay_ms(24);
	PIN_DHT11W1=1;
	Delay_10us();
	Delay_10us();
	Delay_10us();
	Delay_10us();
 
	if(!PIN_DHT11R1)  
	{

		U8FLAG1=2;	 
		while((!PIN_DHT11R1)&&U8FLAG1++);
		U8FLAG1=2;
		while((PIN_DHT11R1)&&U8FLAG1++);	 
		COM1();
		U8RH_data_H_temp1=U8comdata1;
		COM1();
		U8RH_data_L_temp1=U8comdata1;
		COM1();
		U8T_data_H_temp1=U8comdata1;
		COM1();
		U8T_data_L_temp1=U8comdata1;
		COM1();
		U8checkdata_temp1=U8comdata1;
		PIN_DHT11W=1;	 
		U8temp1=(U8T_data_H_temp1+U8T_data_L_temp1+U8RH_data_H_temp1+U8RH_data_L_temp1);
		if(U8temp1==U8checkdata_temp1)
		{
			U8RH_data_H1=U8RH_data_H_temp1;
			U8RH_data_L1=U8RH_data_L_temp1;
			U8T_data_H1=U8T_data_H_temp1;
			U8T_data_L1=U8T_data_L_temp1;
			U8checkdata1=U8checkdata_temp1;
		}

		DHT11Temp1=U8T_data_H1;
		DHT11Damp1=U8RH_data_H1;
				   
	}

}

#endif
