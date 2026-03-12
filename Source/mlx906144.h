
vu16 tempmlx90614 = 0; // 精度到小数点
vu8 cnt90614 = 0;
void mlx90614Deal(vu8 uart) // 100ms调用这个函数
{

	vu8 *a90614, *b90614;
	cnt90614++;
	if (cnt90614 >= 5)
	{
		//			sendstr(uart,"gy906see");
		cnt90614 = 0;
	}
	if (uart == 1)
	{
		a90614 = &Uart_RecOk1;
		b90614 = &Uart_Rx1[0];
	}
	else
	{
		a90614 = &Uart_RecOk2;
		b90614 = &Uart_Rx2[0];
	}

	if (*a90614)
	{
		*a90614 = 0;
		if ((b90614[0] == 'S') && (b90614[5] == 'E')) // 命令设置成功,返回OK
		{
			tempmlx90614 = (b90614[1] - 48) * 100 + (b90614[2] - 48) * 10 + (b90614[4] - 48);
			tempmlx90614 /= 10;
		}
	}
}
