
vu16 sgp30, tvoc; // 二氧化碳浓度,单位ppm
vu8 k30 = 0;

#if 1// 新版本SGP30
void sgp30Deal(vu8 uart, vu8 flag) // flag=1转成百分比
{
	vu8 *asgp30, *bsgp30;
	if (uart == 1)
	{
		asgp30 = &Uart_RecOk1;
		bsgp30 = &Uart_Rx1[0];
	}
	else
	{

		asgp30 = &Uart_RecOk2;
		bsgp30 = &Uart_Rx2[0];
	}

	if (*asgp30)
	{
		*asgp30 = 0;
		if ((bsgp30[0] == 0x2C)) // 真实数据
		{
																									   // 读取哪个数值
						sgp30 = bsgp30[1] * 256 + bsgp30[2];																							   // 读取ppm
			if (flag)
				sgp30 = (sgp30 / 10000.0) * 99.0; // 转换成百分比
		}
	}
}
#else
void sgp30Deal(vu8 uart, vu8 flag) // flag=1转成百分比
{

	vu8 *asgp30, *bsgp30;
	k30++;
	if (k30 > 5)
	{
		k30 = 0;
		//				sendstr(uart,"sgp30see\r\n");
		//		}
		if (uart == 1)
		{
			asgp30 = &Uart_RecOk1;
			bsgp30 = &Uart_Rx1[0];
		}
		else
		{

			asgp30 = &Uart_RecOk2;
			bsgp30 = &Uart_Rx2[0];

		}

		if (*asgp30)
		{
			*asgp30 = 0;
			if ((bsgp30[0] == 'S') && (bsgp30[6] == 'E')) // 命令设置成功,返回OK
			{
				sgp30 = (bsgp30[1] - 48) * 10000 + (bsgp30[2] - 48) * 1000 + (bsgp30[3] - 48) * 100 + (bsgp30[4] - 48) * 10 + (bsgp30[5] - 48);
				tvoc = (bsgp30[6] - 48) * 10000 + (bsgp30[7] - 48) * 1000 + (bsgp30[8] - 48) * 100 + (bsgp30[9] - 48) * 10 + (bsgp30[10] - 48);
				if (sgp30 >= 400)
					sgp30 -= 400;
				if (flag)
				{
					sgp30 = sgp30 / (65536.0 / 99.0);
					tvoc = tvoc / (65536.0 / 99.0);
				}
			}
		}
	}
}
#endif
