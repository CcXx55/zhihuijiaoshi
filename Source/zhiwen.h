
vu8 errcnt = 0;
#define saj2_k1 PAin(2) // 双按键模块的k1脚
#define saj2_k2 PAin(2) // 双按键模块的k2脚
void zhiwenDeal(vu8 uart)
{
	vu8 *azhiwen, *bzhiwen;
	vu16 i;
	if (uart == 1)
	{
		azhiwen = &Uart_RecOk1;
		bzhiwen = &Uart_Rx1[0];
	}
	else
	{
		azhiwen = &Uart_RecOk2;
		bzhiwen = &Uart_Rx2[0];
	}
	if (saj2_k1 == 0) // 删除全部指纹,支持部分
	{
		delayms(10);
		if (saj2_k1 == 0)
		{
			sendchar(uart, 0X5D);
			sendchar(uart, 1);
			sendchar(uart, 1);
		}
		while (saj2_k1 == 0)
			;
	}
	if (saj2_k2 == 0) // 增加指纹,灯亮一个,输入第一次指纹,然后第二个灯亮,等这边显示
					  // Hand2后,再输入,不然容易错误,然后添加指纹成功,2个灯快速闪几下
	{
		delayms(10);
		if (saj2_k2 == 0)
		{
			sendchar(uart, 0X58);
			sendchar(uart, 4);
			//							sendchar(2,1);
		}
		while (saj2_k2 == 0)
			;
	}
	// 记得吧代码放入指纹模块
	if (*azhiwen)
	{
		*azhiwen = 0;

		if (strstr((const char *)&bzhiwen[0], "OK"))
		{
			beep = 0;
			delayms(100);
			beep = 1;
			LCD_DisStr(1, 0, "   Del All OK   ");
			delayms(1000);
			LCD_DisStr(1, 0, "                ");
		}
		else if (strstr((const char *)&bzhiwen[0], "Hand2"))
		{
			beep = 0;
			delayms(100);
			beep = 1;
			LCD_DisStr(1, 0, "     Hand2      ");
		}
		else if ((bzhiwen[0] == 'K') && (bzhiwen[4] == 'K')) // 1~3就是指纹编号
		{
			beep = 0;
			delayms(100);
			beep = 1;
			LCD_DisStr(1, 0, "     Add OK     ");
			delayms(1000);
			LCD_DisStr(1, 0, "                ");
		}
		else if (strstr((const char *)&bzhiwen[0], "Error"))
		{

			beep = 0;
			delayms(100);
			beep = 1;
			errcnt++;
			if (errcnt >= 4)
			{
				LCD_DisStr(1, 0, "     4Error     ");
				beep = 0;
				delayms(1000);
				beep = 1;
			}
			else
				LCD_DisStr(1, 0, "     Error      ");
			delayms(1000);
			LCD_DisStr(1, 0, "                ");
		}
		else if (strstr((const char *)&bzhiwen[0], "Err1"))
		{
			errcnt = 0;

			LCD_DisStr(1, 0, "     Error      ");
			delayms(1000);

			LCD_DisStr(1, 0, "                ");
		}
		else if (strstr((const char *)&bzhiwen[0], "Err2"))
		{
			errcnt = 0;

			LCD_DisStr(1, 0, "     Error      ");
			delayms(1000);

			LCD_DisStr(1, 0, "                ");
		}
		else if ((bzhiwen[0] == 0x47) && (bzhiwen[4] == 0x47)) // 1~3就是指纹编号
		{
			errcnt = 0;
			jdq1 = 0;
			LCD_DisStr(1, 0, "      Open      ");
			delayms(1000);
			jdq1 = 1;
			LCD_DisStr(1, 0, "                ");
		}
		for (i = 0; i < 20; i++)
			bzhiwen[i] = 0;
	}
}
