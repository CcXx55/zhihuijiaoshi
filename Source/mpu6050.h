/*
直接调用mpu6050Deal(3)就行了,数据存储在mpu6050dat结构体

字符串转换成十进制
	float ga1=atof((const char *)&mpu6050dat.jiaoduz[0]);
*/
typedef struct
{
	vu8 jiaodux[6];
	vu8 jiaoduy[6];
	vu8 jiaoduz[6];
	vu8 jiasudux[6];
	vu8 jiasuduy[6];
	vu8 jiasuduz[6];		
}_mpu6050dat_;
_mpu6050dat_ mpu6050dat;
vu8 mpu6050cnt=0;

void mpu6050Deal(vu8 uart)
{
	vu8 *a6050,*b6050,i;

	mpu6050cnt++;
	if(mpu6050cnt>=5)
	{
		mpu6050cnt=0;
		sendstr(uart,"See");
	}
	if(uart==1)
	{
		a6050=&Uart_RecOk1;
		b6050=&Uart_Rx1[0];
	}
	else
	{
		a6050=&Uart_RecOk2;
		b6050=&Uart_Rx2[0];
	}		
	if(*a6050)
	{
		*a6050=0;
		if((b6050[0]=='S') &&(b6050[44]=='E'))
		{
			for(i=0;i<6;i++)
				mpu6050dat.jiaodux[i]=b6050[2+i];
			for(i=0;i<6;i++)
				mpu6050dat.jiaoduy[i]=b6050[9+i];				
			for(i=0;i<6;i++)
				mpu6050dat.jiaoduz[i]=b6050[16+i];

			for(i=0;i<6;i++)
				mpu6050dat.jiasudux[i]=b6050[23+i];
			for(i=0;i<6;i++)
				mpu6050dat.jiasuduy[i]=b6050[30+i];				
			for(i=0;i<6;i++)
				mpu6050dat.jiasuduz[i]=b6050[37+i];				
			
		}
	}
		

}

