/*
	定时器1ms调用wifitime_dingshiqi();
	year 2020则为20
*/
vu8 hour = 0, min = 0, sec = 0, year = 0, month = 0, date = 0, bflag = 0;
vu16 timeoutaaa = 0, kgaaa = 0;

void wifitime_dingshiqi(void)
{

	if (timeoutaaa)
		timeoutaaa--;
	kgaaa++;
	if (kgaaa > 999)
	{
		kgaaa = 0;
		sec++;
		if (sec > 59)
		{
			sec = 0;
			min++;
			if (min > 59)
			{
				min = 0;
				hour++;
				if (hour > 23)
				{
					hour = 0;
					bflag = 0; // 第二天重新同步时间
				}
			}
		}
	}
}
vu8 ga = 0;
void wifitimeDeal(vu8 uart, vu8 *num)
{
	vu8 *atime, *btime;
	if (uart == 1)
	{
		atime = &Uart_RecOk1;
		btime = &Uart_Rx1[0];
	}
	else
	{
		atime = &Uart_RecOk2;
		btime = &Uart_Rx2[0];
	}
	if ((*atime) && (bflag == 0)) // 同步成功后就不去清零串口,外面还用
	{
		*atime = 0;
		if ((btime[0] == 'd') && (btime[1] == 'a'))
		{
			bflag = 1;
			year = (btime[7] - 48) * 10 + (btime[8] - 48);
			month = (btime[10] - 48) * 10 + (btime[11] - 48);
			date = (btime[13] - 48) * 10 + (btime[14] - 48);
			hour = (btime[21] - 48) * 10 + (btime[22] - 48);

			min = (btime[24] - 48) * 10 + (btime[25] - 48);
			sec = (btime[27] - 48) * 10 + (btime[28] - 48);
		}
	}

	if ((timeoutaaa == 0) && (bflag == 0))
	{
		ga = 1 - ga;
		if (ga == 0)
			sendstr(1, num); // 为了吧NUm发送下去
		else
			sendstr(uart, "citime");
		timeoutaaa = 2000;
	}
}
