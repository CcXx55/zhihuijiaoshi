
USART_TypeDef *Guc_USARTx; // 已经验证
#include "uart_xuni.h"
/*******************************************************************************
串口初始化函数,USARTx中的x可以取1~5,代表串口1~5(没有串口0),USART2
*******************************************************************************/
void UARTT_Init(USART_TypeDef *USARTx, vu32 btl)
{
	GPIO_InitTypeDef GPIO_InitStructure1, GPIO_InitStructure2;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	GPIO_TypeDef *GPIOx_UartatN;
	GPIO_TypeDef *GPIOx_UartatM;
	NVIC_InitTypeDef NVIC_InitStructure;
	if (USARTx == USART1)
	{
		/*********打开串口的时钟**************/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_9;	// TX引脚
		GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_10; // RX引脚
		GPIOx_UartatN = GPIOA;						// 代表Tx在PA9
		GPIOx_UartatM = GPIOA;						// 代表Rx在PA10
	}
	else if (USARTx == USART2)
	{
		/*********打开串口的时钟**************/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_2; // Tx引脚
		GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_3; // Rx引脚
		GPIOx_UartatN = GPIOA;					   // 代表Tx在PA2
		GPIOx_UartatM = GPIOA;					   // 代表Rx在PA3
	}
	else if (USARTx == USART3)
	{
		/*********打开串口的时钟**************/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_10; // Tx引脚,1602_D3脚
		GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_11; // Rx引脚,1602_D4脚
		GPIOx_UartatN = GPIOB;						// 代表Tx在PB10
		GPIOx_UartatM = GPIOB;						// 代表Rx在PB11
	}
	else if (USARTx == UART4)
	{
		/*********打开串口的时钟**************/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_10; // Tx引脚
		GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_11; // Rx引脚
		GPIOx_UartatN = GPIOC;						// 代表Tx在PC10
		GPIOx_UartatM = GPIOC;						// 代表Rx在PC11
	}
	else if (USARTx == UART5)
	{
		/*********打开串口的时钟**************/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
		GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_12; // Tx引脚
		GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_2;	// Rx引脚
		GPIOx_UartatN = GPIOC;						// 代表Tx在PC12
		GPIOx_UartatM = GPIOD;						// 代表Rx在PD2
	}

	/*********IO口初始化******************/
	// TX引脚
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_UartatN, &GPIO_InitStructure1);

	// RX引脚
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOx_UartatM, &GPIO_InitStructure2);

	/********串口初始化*******************
	- BaudRate = 115200 baud
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	- USART Clock disabled
	- USART CPOL: Clock is active low
	- USART CPHA: Data is captured on the middle
	- USART LastBit: The clock pulse of the last data bit is not

	output to the SCLK pin*/
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USARTx, &USART_ClockInitStructure);

	USART_InitStructure.USART_BaudRate = btl;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* Configure USART1 basic and asynchronous paramters */
	USART_Init(USARTx, &USART_InitStructure);

	/* Enable USART1 */
	USART_Cmd(USARTx, ENABLE);

	if (USARTx == USART1)
	{
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		// 默认使用串口1
		Guc_USARTx = USARTx;
		//    /* enabling interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = 37;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	else if (USARTx == USART2)
	{
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		// 默认使用串口1
		Guc_USARTx = USARTx;
		//    /* enabling interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = 38;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	else if (USARTx == USART3)
	{
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		// 默认使用串口1
		Guc_USARTx = USARTx;
		//    /* enabling interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = 39;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}

// 设置要使用哪个串口(取值USART1~USART5,没有串口0),设置好后可用USART_GetDate函数
// 或者stdio库中printf函数
void Usart_Set(USART_TypeDef *USARTx)
{
	Guc_USARTx = USARTx;
}

// 得到一个串口数据,需要调用UsartSet函数来设置要使用哪个串口
vu8 USART_GetDate(void)
{
	while (USART_GetFlagStatus(Guc_USARTx, USART_IT_RXNE) == RESET)
		;
	// 清除接收完成标志,如果是连续接收数据,可以不用清除
	// USART_ClearFlag(USART2,USART_IT_RXNE);
	return USART_ReceiveData(Guc_USARTx);
}
extern void sendchar4(vu8 b);
// 兼容51
void sendchar(vu8 b, vu8 a)
{
	if (b == 1)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		// 发送数据
		USART_SendData(USART1, (u8)a);
	}
	else if (b == 2)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		// 发送数据
		USART_SendData(USART2, (u8)a);
	}
	else if (b == 3)
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		// 发送数据
		USART_SendData(USART3, (u8)a);
	}

	else if (b == 4)
	{
		sendchar4(a); // 虚拟的串口4
	}
	else if (b == 5) // 虚拟的串口5
	{
		sendchar5(a);
	}
}

void sendstr(vu8 b, vu8 *a)
{
	while (*a)
		sendchar(b, *a++);
}
#define wifi_send sendchar

#define out 0
#define od 1
#define in 2
#define PA 0
#define PB 1
#define PC 2
#define PD 3

/*用這個好处代码整洁
   set(PA,2,out)//让PA2输入,函数有自动启动上拉功能
		PA取值PA-PD
		out取值out,in,od(开漏输出)
*/
void set(vu8 duankou, vu8 pin, vu8 mode) //
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = 1 << pin;
	if (mode == out)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	else if (mode == od)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	else if (mode == in)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if (duankou == PA)
	{
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		PAout(pin) = 1;
	}
	if (duankou == PB)
	{
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		PBout(pin) = 1;
	}
	if (duankou == PC)
	{
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		PCout(pin) = 1;
	}
	if (duankou == PD)
	{
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		PDout(pin) = 1;
	}
}

void iow(vu8 port1, vu8 bitt, vu8 state, vu8 value)
{

	u32 addr; // 代表用PA口,ODR代表输出
	set(port1, bitt, state);
	if (port1 == PA)
		addr = GPIOA_ODR_Addr; // 代表用PA口,ODR代表输出
	else if (port1 == PB)

		addr = GPIOB_ODR_Addr; // 代表用PB口,ODR代表输出
	else if (port1 == PC)

		addr = GPIOC_ODR_Addr; // 代表用PC口
	BIT_ADDR(addr, bitt) = value;
}
vu8 iow_r(vu8 port1, vu8 bitt) // 读取IO口输出
{

	u32 addr; // 代表用PA口,ODR代表输出

	if (port1 == PA)
		addr = GPIOA_ODR_Addr; // 代表用PA口,ODR代表输出
	else if (port1 == PB)

		addr = GPIOB_ODR_Addr; // 代表用PB口,ODR代表输出
	else if (port1 == PC)

		addr = GPIOC_ODR_Addr; // 代表用PC口
	return BIT_ADDR(addr, bitt);
}

// zu代表IO电平放数组zu元素
void iow_qufan(vu8 port1, vu8 bitt, vu8 state)
{
	vu8 ia = 0;
	// set(port1, bitt, state);//不能设置不然强制为1,然后读错值,但是要么响要么不响,下次就正常了

	ia = iow_r(port1, bitt);
	iow(port1, bitt, state, 1 - ia);
	//	u32 addr; // 代表用PA口,ODR代表输出
	//
	//	set(port1, bitt, state);
	//	if (port1 == PA)
	//		addr = GPIOA_ODR_Addr; // 代表用PA口,ODR代表输出
	//	else if (port1 == PB)

	//		addr = GPIOB_ODR_Addr; // 代表用PB口,ODR代表输出
	//	else if (port1 == PC)

	//		addr = GPIOC_ODR_Addr; // 代表用PC口
	//	bita[zu] = 1 - bita[zu];
	//	BIT_ADDR(addr, bitt) = bita[zu];
}
// 读取端口输出的值(不是输入)
vu8 ior(vu8 port1, vu8 bitt)
{

	u32 addr; // 代表用PA口,ODR代表输出
	set(port1, bitt, in);
	if (port1 == PA)
		addr = GPIOA_IDR_Addr; // 代表用PA口,ODR代表输出
	else if (port1 == PB)

		addr = GPIOB_IDR_Addr; // 代表用PB口,ODR代表输出
	else if (port1 == PC)

		addr = GPIOC_IDR_Addr; // 代表用PC口
	return BIT_ADDR(addr, bitt);
}
