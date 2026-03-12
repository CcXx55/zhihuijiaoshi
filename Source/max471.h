vu16 dianliu = 0;//电流,单位ma
vu8 dianliucnt = 0;
vu32 dianliusum = 0;
		

	// 显示数值,单位ma
void max471_deal(void)
{	
if(xiaozhunsys==0XFFFF)//如果没有校准,就强制100
		xiaozhunsys=100;
			//max471输出1V,就是1000mv,就是对应1000mA
	dianliusum += (GetADCResult(max471_adcIO, 0) / 4096.0*(100.0/xiaozhunsys) * powervol  * 1000.0);
 
			 
	if (dianliucnt++ >= 10)//10次求平均,看显示的效果OK
	{
		dianliu = dianliusum / dianliucnt;//求平均
		dianliucnt = 0;//清零计数
		dianliusum = 0;//清零和
	}

			if(keyread_xiaodou(xiaozhunkey_port,xiaozhunkey_bit)==0)//校准键,用100mA
	{
		dianliusum=0;//清零和

		for(i=0; i<100; i++) //循环采样100次
		{

							//max471输出1V,就是1000mv,就是对应1000mA
			dianliusum += (GetADCResult(max471_adcIO, 0) / 4096.0 * powervol* 1000.0  );
							delayms(10);//延时10ms
		}
		sys[9]=dianliusum/100.0;//保存校准值
		SysSave();
		dianliusum=0;//清零和
		dianliucnt=0;//清零计数
	}
  


}