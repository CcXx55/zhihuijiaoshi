extern void LCD_Dis4bit(vu8 Line, vu8 Row, vu8 *sa, vu16 value1);
vu16 heigh = 0;
#define saj1_k1 PAin(2)	 // 双按键模块的k1脚,要初始化成in
#define saj1_k2 PAin(2)	 // 双按键模块的k2脚,要初始化成in
void hx711Deal(vu8 uart) // 100ms调用这个函数
{

	vu8 *a711, *b711;
	if (uart == 1)
	{
		a711 = &Uart_RecOk1;
		b711 = &Uart_Rx1[0];
	}
	else
	{

		a711 = &Uart_RecOk2;
		b711 = &Uart_Rx2[0];

	}


	if (*a711)
	{
		*a711 = 0;

		if ((b711[6] == 'e') && (b711[7] == 't'))
		{
			heigh = (b711[9] - 48) * 10000 + (b711[10] - 48) * 1000 + (b711[11] - 48) * 100 + (b711[12] - 48) * 10 + (b711[13] - 48);
			//LCD_Dis4bit(1, 0, "H:", heigh);
		}
	}
}
