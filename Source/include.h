#include "stm32f10x_lib.h"
#define u8 unsigned char
#define u16 unsigned int
#define uchar u8
#define uint u16

// IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// IO口地址映射
#define GPIOA_ODR_Addr (GPIOA_BASE + 12) // 0x4001080C
#define GPIOB_ODR_Addr (GPIOB_BASE + 12) // 0x40010C0C
#define GPIOC_ODR_Addr (GPIOC_BASE + 12) // 0x4001100C
#define GPIOD_ODR_Addr (GPIOD_BASE + 12) // 0x4001140C
#define GPIOE_ODR_Addr (GPIOE_BASE + 12) // 0x4001180C
#define GPIOF_ODR_Addr (GPIOF_BASE + 12) // 0x40011A0C
#define GPIOG_ODR_Addr (GPIOG_BASE + 12) // 0x40011E0C

#define GPIOA_IDR_Addr (GPIOA_BASE + 8) // 0x40010808
#define GPIOB_IDR_Addr (GPIOB_BASE + 8) // 0x40010C08
#define GPIOC_IDR_Addr (GPIOC_BASE + 8) // 0x40011008
#define GPIOD_IDR_Addr (GPIOD_BASE + 8) // 0x40011408
#define GPIOE_IDR_Addr (GPIOE_BASE + 8) // 0x40011808
#define GPIOF_IDR_Addr (GPIOF_BASE + 8) // 0x40011A08
#define GPIOG_IDR_Addr (GPIOG_BASE + 8) // 0x40011E08

// IO口操作,只对单一的IO口!
// 确保n的值小于16!
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n) // 输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)  // 输入

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n) // 输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)  // 输入

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n) // 输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)  // 输入

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr, n) // 输出
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr, n)  // 输入

#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr, n) // 输出
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr, n)  // 输入

#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr, n) // 输出
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr, n)  // 输入

#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr, n) // 输出
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr, n)  // 输入

#define uartwifi 1
#define oleduart 2

#define TIM TIM1                        // TIM声明
#define RCC_APB_TIM RCC_APB2Periph_TIM1 // TIM时钟使能

extern void byte_read(uint byte_addr, uint i, uchar *p);
extern void byte_write(uint sector_addr, uint byte_addr, uint i, uchar *shujui_eeprom);

extern u16 GetTemp(void);
extern void Delay_NMS(vu16 z);
extern void DH11_GetTempDamp(vu8 port, vu8 bit1);
extern u8 TempNow, DampNow;
extern void LCD_Init(void);
extern void LCD_Dis(u8 Line, u8 Row, u8 Dat);
extern void LCDW_Com(u8 com);
extern void LCDW_Dat(u8 dat);
// extern uint DS18B20_GetTemp(void);//温度转换
extern u8 SysMode;
extern u8 TempNow; // 当前温度
extern void LCD_DisStr(u8 Line, u8 Row, vu8 *Dat);

#define PIN_LED_Fac PBout(9)
#define PIN_LED_Warn PBout(8)

vu8 Uart_Rx1[280], UartTx[100] = {0x58};
vu16 RecCnt;
vu16 RecTimeCount;
vu8 UartRecInit = 0, Uart_RecOk = 0, UartFlag_RecTime = 0;

#define RecCnt1 RecCnt
#define UartRx Uart_Rx1
#define UartRx1 Uart_Rx1

#define UartRecOk Uart_RecOk
#define UartRecOk1 Uart_RecOk

#define Uart_RecOk1 Uart_RecOk

vu8 Uart_Rx2[280], UartTx2[100] = {0x58};
vu16 RecCnt2;
vu16 RecTimeCount2;
vu8 UartRecInit2 = 0, Uart_RecOk2 = 0, UartFlag_RecTime2 = 0;

vu8 Uart_Rx3[280], UartTx3[100] = {0x58};
vu16 RecCnt3;
vu16 RecTimeCount3;
vu8 UartRecInit3 = 0, Uart_RecOk3 = 0, UartFlag_RecTime3 = 0;
#include "string.h"
#include "stdio.h"
///******************************************************************************
// 本函数为printf的底层函数,不要改变其函数名和输入参数名,有了这函数,并且option for
//  targe->targe->Use MicroLIB勾上,可以像51单片机那样使用printf函数(直接写printf,不需TI=1,while(!TI)等等)
//******************************************************************************/
int fputc(int ch, FILE *f)
{
    // 等待发送完毕,必须放在前面
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;
    // 发送数据
    USART_SendData(USART1, (u8)ch);
    return ch;
}
// 从s字符串找到字段name的值,若返回值为0没找到或值为0,1找到,把数据存入dat指针里
//  datmaxlen代表数据最大值
/*********************
dats:122 dat=13 aa='1' bb="" cc= ka=2 dats:"122"\r\n 这个函数可以准确找到每个字段的值
*************/
vu8 Recmsg_getval(vu8 *s, vu8 *name, vu8 *dat, vu16 datmaxlen)
{
    vu16 i, j, len = strlen((const char *)s), k;

    for (i = 0; i < len; i++)
    {
        if (s[i] == name[0])
        {
            j = 0;

            while (*(s + i + j))
            {
                if (*(s + i + j) != name[j])
                    break;

                j++;
            }

            if (j == strlen((const char *)name)) // 找到字段
            {
                if ((s[i + j] != '=') && (s[i + j] != ':') & (s[i + j] != ',')) // 防止字段dat,dats冲突,所以字段值=或者:
                    ;
                else
                {
                    i++;

                    if ((s[i + j] == 34) && (s[i + j + 1] == 34)) // 返回值为""认为没数据
                    {
                        dat[0] = 0;
                        return 0;
                    }
                    else if ((s[i + j] == 39) && (s[i + j + 1] == 39)) // 返回值为''认为没数据
                    {
                        dat[0] = 0;
                        return 0;
                    }
                    else
                    {
                        if ((s[i + j] == 34) || (s[i + j] == 39)) // 数据可能以单引号或双引号开头
                            i++;

                        for (k = 0; k < datmaxlen; k++) // 要限制长度,不然超了会溢出
                        {

                            if ((s[j + i + k] == ',') || (s[j + i + k] == ' ') || (s[j + i + k] == 34) || (s[j + i + k] == 39) || (s[j + i + k] == '\r'))
                                break;

                            *dat++ = s[j + i + k];
                        }

                        *dat++ = 0;
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

vu32 strtohex(vu8 *zz, vu8 *bb, vu16 *zz1) // 比如dat11字符串"999"或"9",会转成正确的十进制
{
    vu8 i;
    vu32 j = *zz1;
    for (i = 0; i < 30; i++)
    {
        if ((zz[i] == bb[0]) && (zz[i + 1] == bb[1]))
        {
            i += 5;
            if ((zz[4 + i] > 47) && (zz[4 + i] < 58))                                                                                         // 如果数据为5位
                j = ((zz[0 + i] - 48) * 10000 + (zz[1 + i] - 48) * 1000 + (zz[2 + i] - 48) * 100 + (zz[3 + i] - 48) * 10 + (zz[4 + i] - 48)); // 吧数组的值存入sys数组
            else if ((zz[3 + i] > 47) && (zz[3 + i] < 58))                                                                                    // 如果数据为4位
                j = ((zz[0 + i] - 48) * 1000 + (zz[1 + i] - 48) * 100 + (zz[2 + i] - 48) * 10 + (zz[3 + i] - 48));                            // 吧数组的值存入sys数组
            else if ((zz[2 + i] > 47) && (zz[2 + i] < 58))                                                                                    // 如果数据为3位
                j = ((zz[0 + i] - 48) * 100 + (zz[1 + i] - 48) * 10 + (zz[2 + i] - 48));                                                      // 吧数组的值存入sys数组
            else if ((zz[1 + i] > 47) && (zz[1 + i] < 58))                                                                                    // 如果数据为2位
                j = ((zz[0 + i] - 48) * 10 + (zz[1 + i] - 48));                                                                               // 吧数组的值存入sys数组
            else
                j = ((zz[0 + i] - 48)); // 个位数直接填入
        }
    }

    return j;
}
vu8 dat11[120];

vu16 sys[20];
#define led PBout(12)
#define led1 led
#define led2 PBout(13)

#if 1 // 取1则48脚最小PCB板,否则洞洞板
#define keys PAin(8)
	#define key1 PBin(8)
	#define key2 PBin(9)
#else
	#define keys iow_r(PB,11)
	#define key1 iow_r(PB,0)
	#define key2 iow_r(PB,1)

#endif
#define k1 keys
#define k2 key1
#define k3 key2

#define Beep PAout(11)
#define beep Beep
#define temp DHT11Temp
#define damp DHT11Damp
#define TempNow DHT11Temp
#define DampNow DHT11Damp

#define jdq PAout(2)
#define jdq1 jdq
#include <stdlib.h>
