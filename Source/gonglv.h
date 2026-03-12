
vu16 dianliu, dianya, PA1;
vu8 yinsu; // 功率因素
vu8 cntgonglv = 0;
#define PAsum sys[9]
void gonglv(vu8 uartgonglv) // 读取功率因素
{
	vu8 *agonglv, *bgonglv;
	cntgonglv++;
	if (uartgonglv == 1) // 如果使用串口1
	{
		agonglv = &Uart_RecOk1; // 串口1接收完成标志
		bgonglv = &Uart_Rx1[0]; // 串口1接收的数据组
	}
	else
	{
		agonglv = &Uart_RecOk2; // 串口2接收完成标志
		bgonglv = &Uart_Rx2[0]; // 串口2接收的数据组
	}
	if (cntgonglv > 10) // 定时串口发送指令的命令,1秒
	{
		cntgonglv = 0;			 // 清零变量
		sendchar(uartgonglv, 1); // 记得4800波特率,这是给模块发送命令,1代表数据1,3代表数据3,模块会返回功率等值
		sendchar(uartgonglv, 3);
		sendchar(uartgonglv, 0);
		sendchar(uartgonglv, 0x48);
		sendchar(uartgonglv, 0);
		sendchar(uartgonglv, 0x0a);
		sendchar(uartgonglv, 0x45);
		sendchar(uartgonglv, 0xDB);
	}
	if (*agonglv) // 收到串口数据
	{
		*agonglv = 0;									  // 清零数据
		if ((bgonglv[0] == 0x01) && (bgonglv[1] == 0x03)) // 如果数据是1,3开头
		{
			dianya = (bgonglv[3] * 256 + bgonglv[4]) / 100; // 功率
			dianliu = bgonglv[5] * 256 + bgonglv[6];		// 电流
			PA1 = bgonglv[7] * 256 + bgonglv[8];			// 电压

			yinsu = (bgonglv[13] * 256 + bgonglv[14]) / 1000.0 * 100.0; // 功率因素,比如98,代表0.98,最大100
#if 0
			PAsum = bgonglv[9] *65536* 256 + bgonglv[10]*65536+bgonglv[11]*256+bgonglv[12];			// 电压

#else
			PAsum += PA1; // 满65535,自动清零,能理解
			SysSave();
#endif
		}
	}
}

