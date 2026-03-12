#define Searchlen 88

#include "string.h"
void SendUart(vu8 a)
{

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	// ·¢ËÍÊý¾Ý
	USART_SendData(USART1, (u8)a);
}

void SendUartNoOK(vu8 *s)
{

	while (*s)
	{
		SendUart(*s++);
	}
}

vu8 SearchStr(vu8 *s, vu8 Len) //??????????s?????,Len???????,?????1
{
	vu16 i, j;
	for (i = 0; i < Searchlen; i++)
	{
		if (UartRx[i] == s[0])
		{
			j = 0;
			while (*(s + j))
			{
				if (*(s + j) != UartRx[i + j])
					break;
				j++;
			}
			if (j == Len)
				return 1;
		}
	}
	return 0;
}

//???????????
void ClrAllRxBuf(void)
{
	vu8 i;
	Uart_RecOk = 0;
	for (i = 0; i < Searchlen; i++)
		UartRx[i] = 0;
}
//????command,??ack??,??????timeout,??abortCnt,???,??0,??ACK1??1,ACK2??2,????
//?????????,??????????????UartRx,?????wifi
vu8 GPRSSendWaitACK(vu8 *command, const char *ack1, const char *ack2, vu16 timeout, vu8 abortCnt)
{
	vu16 i, j = 0, ls = 0;

	while (1)
	{

		ClrAllRxBuf();
		i = 0;

		SendUartNoOK(command);
		SendUart('\r');
		SendUart('\n');
		while (i++ < timeout)
		{
			delay_ms(1);
			if (Uart_RecOk)
				break;
		}

		if (Uart_RecOk == 1)
		{
			Uart_RecOk = 0;

			if (SearchStr((vu8 *)ack1, strlen(ack1)))
				return 1;
			else if (SearchStr((vu8 *)ack2, strlen(ack2)))
				return 2;
		}
		else
			ls++;

		j++;
		if (j >= abortCnt)
			break;
		if (i < timeout)
			delay_ms(100);
	}

	ClrAllRxBuf();
	return 0;
}

vu8 duanxinsend(vu8 *p2)
{
	vu16 i = 0;

	if (GPRSSendWaitACK("AT+CMGS=\"17878761793\"", ">", "dghsh", 1000, 1)) //??????+????
	{

		ClrAllRxBuf();
		SendUartNoOK(p2); //???????GSM??

		SendUart(0x1a);
	}
	else
		return 8;
	return 0;
}

void sim800init(void)
{

	while (GPRSSendWaitACK("AT+CSCS=\"GSM\"", "OK", "1356", 300, 1) == 0)
		;

	while (GPRSSendWaitACK("AT+CMGF=1", "OK", "dghsh", 300, 1) == 0)
		;
	//??????????????
	while (GPRSSendWaitACK("AT+CNMI=2,2,0,0,0", "OK", "dghsh", 300, 1) == 0)
		;
	//   	flaginit=1;

	//   while(GPRSSendWaitACK("AT+CMGF=0","OK","dghsh",3000,1)==0);

	//    while(GPRSSendWaitACK("AT+CSMP=17,167,0,8","OK","ERROR",3000,1)==0);

	//    while(GPRSSendWaitACK("AT+CSCS=\"UCS2\"","OK","ERROR",3000,1)==0);
}
