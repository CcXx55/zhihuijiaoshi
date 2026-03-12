vu16 bh1750 = 0; // 单位mm
vu8 cntbh1750 = 0;

vu8 bh1750Deal(vu8 uart) // 100ms调用这个函数
{

	vu8 *abh1750, *bbh1750;
	cntbh1750++;
	if (cntbh1750 >= 5)
	{
		cntbh1750 = 0;
		sendstr(uart, "bh1750see");
	}
	if (uart == 1)
	{
		abh1750 = &Uart_RecOk1;
		bbh1750 = &Uart_Rx1[0];
	}
	else
	{
		abh1750 = &Uart_RecOk2;
		bbh1750 = &Uart_Rx2[0];
	}

	if (*abh1750)
	{
		*abh1750 = 0;
		if ((bbh1750[0] == 'S') && (bbh1750[6] == 'E'))
		{

			bh1750 = (bbh1750[1] - 48) * 10000 + (bbh1750[2] - 48) * 1000 + (bbh1750[3] - 48) * 100 + (bbh1750[4] - 48) * 10 + (bbh1750[5] - 48);
		}
	}
	return 0;
}
