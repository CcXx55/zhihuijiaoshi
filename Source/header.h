extern void SysRead(void);
extern void SysSave(void);
vu8 DHT11Temp, DHT11Damp;
#include "stmflash.h"
#include "delay.h"
#include "Uart.h"
// #include "dh.h"
#include "adc.h"
// #include "18B20.h"
//  #include "lanya.h"
// #include "duoji.h"
// #include "motor.h"
//  #include "gps.h"
//  #include "jq8400.h"
//  #include "chaoshengbo.h"
//  #include "chaoshengbo_uart.h"
// #include "wifitime.h" //网络同步时间
//  #include "pm25.h"
//  #include "max30100.h"
//  #include "mlx906144.h"
//  #include "1602.h"

// #include "uartint.h" //串口+定时器中断
#include "display.h"
// #include "nvd040.h"
//  #include "sgp30.h"
//  #include "sht30.h"
//  #include "bh1750.h"

// #include "exit.h"//外部中断
#include "wifi.h"
// #include "huoer.h"
//  #include "rtc.h"
//  #include "zhiwen.h"
//  #include "juzhenjianpan.h"
//  #include "adc0832.h"
//  #include "shuiwei.h"
//  #include "hx711.h"
//  #include "rc522.h" //试用于48脚的stm32,调用init_rc522(void),剩下去印象抄
//  #include "keyboard.h"

// #include "mpu6050.h"
// #include "nrf24l01.h"

// #include "l298.h"
void uartsend_shebei(vu8 uart, vu8 value1)
{
	sendchar(uart, value1 + 48);
}
void uartsend_1bit(vu8 uart, vu8 value1)
{

	sendchar(uart, value1);
}

void uartsend_2bit(vu8 uart, vu8 value1)
{
	sendchar(uart, value1 / 10 + 48);
	sendchar(uart, value1 % 10 + 48);
}
void uartsend_2bit_vol(vu8 uart, vu8 value1)
{

	sendchar(uart, value1 / 10 + 48);
	sendchar(uart, '.');
	sendchar(uart, value1 % 10 + 48);
}
void uartsend_3bit(vu8 uart, vu16 value1)
{
	sendchar(uart, value1 / 100 + 48);
	sendchar(uart, value1 / 10 % 10 + 48);
	sendchar(uart, value1 % 10 + 48);
}
void uartsend_3bit_ph(vu8 uart, vu16 value1)
{
	sendchar(uart, value1 / 100 + 48);
	sendchar(uart, value1 / 10 % 10 + 48);
	sendchar(uart, '.');
	sendchar(uart, value1 % 10 + 48);
}
void uartsend_4bit(vu8 uart, vu16 value1)
{
	sendchar(uart, value1 / 1000 + 48);
	sendchar(uart, value1 / 100 % 10 + 48);
	sendchar(uart, value1 / 10 % 10 + 48);
	sendchar(uart, value1 % 10 + 48);
}
void uartsend_5bit(vu8 uart, vu32 value1)
{
	sendchar(uart, value1 / 10000 + 48);
	sendchar(uart, value1 / 1000 % 10 + 48);
	sendchar(uart, value1 / 100 % 10 + 48);
	sendchar(uart, value1 / 10 % 10 + 48);
	sendchar(uart, value1 % 10 + 48);
}

void uartinitirq(void)
{

	if (UartFlag_RecTime) // 从接收第一个有效字节开始,进行一秒的倒计时,如果没收到完整的
						  // 复位接收器,并发送一个字节的报错数据0x88.
	{
		if (RecTimeCount > 0)
			RecTimeCount--;
		else
		{
			UartRecInit = 0;
			UartFlag_RecTime = 0; // 禁止接收倒计时
			Uart_RecOk = 1;
		}
	}
	if (UartFlag_RecTime2) // 从接收第一个有效字节开始,进行一秒的倒计时,如果没收到完整的
						   // 复位接收器,并发送一个字节的报错数据0x88.
	{
		if (RecTimeCount2 > 0)
			RecTimeCount2--;
		else
		{
			UartRecInit2 = 0;
			UartFlag_RecTime2 = 0; // 禁止接收倒计时
			Uart_RecOk2 = 1;
		}
	}
	if (UartFlag_RecTime3) // 从接收第一个有效字节开始,进行一秒的倒计时,如果没收到完整的
						   // 复位接收器,并发送一个字节的报错数据0x88.
	{
		if (RecTimeCount3 > 0)
			RecTimeCount3--;
		else
		{
			UartRecInit3 = 0;
			UartFlag_RecTime3 = 0; // 禁止接收倒计时
			Uart_RecOk3 = 1;
		}
	}
}

/*******************************************************************************
 * Function Name  : USART1_IRQHandler
 * Description    : This function handles USART1 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART1_IRQHandler(void)
{
	vu8 dat;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		dat = USART1->DR;
#ifdef _lanya_
		if (dat == 0)
			return;
#endif
		if (UartRecInit == 0)
		{
			UartRecInit = 1;
			RecCnt = 0;			  // 接收个数
			UartFlag_RecTime = 1; // 允许接收倒计时
		}
		RecTimeCount = 30; // 接收倒计时时间,1秒
		if (RecCnt < 280)
			Uart_Rx1[RecCnt++] = dat;

		if (RecCnt >= 280)

		{
			UartRecInit = 0;
			RecTimeCount = 100;	  // 防程序偶合,没多大意义
			UartFlag_RecTime = 0; // 禁止接收倒计时
			Uart_RecOk = 1;
		}
	}
}
/*******************************************************************************
 * Function Name  : USART1_IRQHandler
 * Description    : This function handles USART1 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART2_IRQHandler(void)
{
	vu8 dat;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{

		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

		dat = USART2->DR;
#ifdef _lanya_
		if (dat == 0)
			return;
#endif
		if (UartRecInit2 == 0)
		{
			UartRecInit2 = 1;
			RecCnt2 = 0;		   // 接收个数
			UartFlag_RecTime2 = 1; // 允许接收倒计时
		}
		RecTimeCount2 = 30; // 接收倒计时时间,1秒
		if (RecCnt2 < 280)
			Uart_Rx2[RecCnt2++] = dat;

		if (RecCnt2 >= 280)

		{
			UartRecInit2 = 0;
			RecTimeCount2 = 100;   // 防程序偶合,没多大意义
			UartFlag_RecTime2 = 0; // 禁止接收倒计时
			Uart_RecOk2 = 1;
		}
	}
}

/*******************************************************************************
 * Function Name  : USART1_IRQHandler
 * Description    : This function handles USART1 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART3_IRQHandler(void)
{
	vu8 dat;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{

		USART_ClearITPendingBit(USART3, USART_IT_RXNE);

		dat = USART3->DR;
#ifdef _lanya_
		if (dat == 0)
			return;
#endif
		if (UartRecInit3 == 0)
		{
			UartRecInit3 = 1;
			RecCnt3 = 0;		   // 接收个数
			UartFlag_RecTime3 = 1; // 允许接收倒计时
		}
		RecTimeCount3 = 30; // 接收倒计时时间,1秒
		if (RecCnt3 < 280)
			Uart_Rx3[RecCnt3++] = dat;

		if (RecCnt3 >= 280)

		{
			UartRecInit3 = 0;
			RecTimeCount3 = 100;   // 防程序偶合,没多大意义
			UartFlag_RecTime3 = 0; // 禁止接收倒计时
			Uart_RecOk3 = 1;
		}
	}
}

extern void uart_xuni(void);
// 调用Exit_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_IPU, EXTI_Trigger_Rising, 2, 5);
// 就可以进入外部中断函数EXTI9_5_IRQHandler(void)
