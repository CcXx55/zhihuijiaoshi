#include "exit.h"

// 虚拟串口发不能到虚拟串口收,因为发的电平时间经常跳动,但是跟其他模块或PC都OK
// 这个接收数据要效验,不然刚上电或插拔USB会让中断误判断从而有收到字节
// 定时1默认还是100us,有用到外部中断会自动变成104us(100us不行)
/*
    串口时序:一直为高,同步头1位也是104us,然后8位数据(比如2个0,就是2个104us低电平),
    然后1位高电平(104us),再加3位连续高电平(共104*3us)
  现在同步时间用DS1302,走时都1302完成,所以,其他方面不用很准,可以经常清零定时器0
    因为有可能2个位一直都为低,必须外部中断作同步信号,定时器读数据
*/
#define RX4 PAin(1) // 串口4的接收脚,记得调用UART4_INIT函数
#define TX4 PAout(0)

#define RX5 PAin(5) // 串口5的接收脚,记得调用UART5_INIT函数
#define TX5 PAout(4)

// 虚拟串口发不能到虚拟串口收,因为发的电平时间经常跳动,但是跟其他模块或PC都OK
// 这个接收数据要效验,不然刚上电或插拔USB会让中断误判断从而有收到字节
void UART4_INIT(void) // 虚拟串口4
{
    set(PA, 0, out);                                                                // TX4脚
    set(PA, 1, in);                                                                 // RX4脚
                                                                                    // 串口4的接收脚
    Exit_Init(GPIOA, GPIO_Pin_1, GPIO_Mode_IPU, EXTI_Trigger_Rising_Falling, 0, 0); // 必须上升下降沿,不然不能用
}
void UART5_INIT(void) // 虚拟串口5
{
    set(PA, 4, out); // TX5脚
    set(PA, 5, in);  // RX5脚
                     // 串口5的接收脚
    Exit_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_IPU, EXTI_Trigger_Rising_Falling, 0, 0);
}

// 其他外部中断函数在it.c
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) // 检查指定的EXTI1线路触发请求发生与否
    {
        EXTI_ClearITPendingBit(EXTI_Line1); // 清除EXTI0线路挂起位
        if (!RX4)                           // 如果接收脚是低
        {
            if (startrec4 == 0) // 第一次接收
            {
                TIM1_XinInit(); // 定时器1从0重新计时(并初始化成104us),不然容易错位
                startrec4 = 1;  // 启动接收程序
                RDAT4 = 0;
                RBIT4 = 9; // 接收数据的位长度为9
            }
        }
    }
}
// 其他外部中断函数在it.c
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) // 检查指定的EXTI1线路触发请求发生与否
    {
        EXTI_ClearITPendingBit(EXTI_Line5); // 清除EXTI0线路挂起位
        if (!RX5)                           // 如果接收脚是低
        {
            if (startrec5 == 0) // 第一次接收
            {
                TIM1_XinInit(); // 定时器1从0重新计时(并初始化成104us),不然容易错位
                startrec5 = 1;  // 启动接收程序
                RDAT5 = 0;
                RBIT5 = 9; // 接收数据的位长度为9
            }
        }
    }
}
// 定时器1已调用,有触发外部中断,定时器1变104us
// 100us不可用
void uart_xuni(void)
{

    if (xuni_timeout4)
    {
        xuni_timeout4--;
        if (xuni_timeout4 == 0) // 数据间隔是500ms发送,循环100ms,肯定能在新的一串处理完,所以不必再多写代码,浪费时间
            Uart_RecOk4 = 1;
    }

    if (startrec4) // 这个判断是否有低电平,有自动变高
    {

        RDAT4 >>= 1;       // 接收是一位位收
        if (RX4)           // 如果数据为高,
            RDAT4 |= 0x80; // 数据位就置位1
        RBIT4--;
        if (RBIT4 == 0)
        {
            startrec4 = 0;     // 关闭接收
            if (RecCnt4 < 100) // 吧数据存储缓存区 RDAT4存入数组
                Uart_Rx4[RecCnt4++] = RDAT4;
            xuni_timeout4 = 10 * 10; // 字节10ms超时
        }
    }

    if (xuni_timeout5)
    {
        xuni_timeout5--;
        if (xuni_timeout5 == 0) // 数据间隔是500ms发送,循环100ms,肯定能在新的一串处理完,所以不必再多写代码,浪费时间
            Uart_RecOk5 = 1;
    }
    if (startrec5) // 这个判断是否有低电平,有自动变高
    {
        RDAT5 >>= 1;       // 接收是一位位收
        if (RX5)           // 如果数据为高,
            RDAT5 |= 0x80; // 数据位就置位1
        RBIT5--;
        if (RBIT5 == 0)
        {
            startrec5 = 0;     // 关闭接收
            if (RecCnt5 < 100) // 吧数据存储缓存区 RDAT4存入数组
                Uart_Rx5[RecCnt5++] = RDAT5;
            xuni_timeout5 = 10 * 10; // 字节10ms超时
        }
    }
}

void sendchar4(vu8 b)
{
    vu8 i = 0;
    TX4 = 0; // 拉低发射脚
    delay_us(104);
    for (i = 0; i < 8; i++)
    {
        TX4 = b & 1; // 一位一位发送
        b >>= 1;     // 右移
        delay_us(104);
    }
    TX4 = 1; // 拉高发射脚
    delay_us(104);
    TX4 = 1; // 拉高发射脚
    delay_us(104);
    TX4 = 1; // 拉高发射脚
    delay_us(104);
}

void sendchar5(vu8 b)
{
    vu8 i = 0;
    TX5 = 0; // 拉低发射脚
    delay_us(104);
    for (i = 0; i < 8; i++)
    {
        TX5 = b & 1; // 一位一位发送
        b >>= 1;     // 右移
        delay_us(104);
    }
    TX5 = 1; // 拉高发射脚
    delay_us(104);
    TX5 = 1; // 拉高发射脚
    delay_us(104);
    TX5 = 1; // 拉高发射脚
    delay_us(104);
}
