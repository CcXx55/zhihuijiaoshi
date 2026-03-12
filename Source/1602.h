
// #define lcd12864
//#define pcbxiao //用吧V60改成STM32最小系统的版本
//#define dongdongban //耀森洞洞板,记得改按键
extern void delay_ms(u16 nms);

#ifndef dongdongban//PCB板

	#define PIN_LCD_RS PAout(6)
	#define PIN_LCD_ENa PAout(7)
/***********************************************************************************************
函数名:	 往1602写命令
输入参数: com:命令值
输出:无
说明:无
***********************************************************************************************/
void LCDW_Com(u8 dat)
{
    vu8 n = 0, s = 0,g=0;;

    PIN_LCD_RS = 0;
    delay_ms(1);

    if ((dat & 0x01) == 0x01)

        PBout(0) = 1;

    else
        PBout(0) = 0;

    if ((dat & 0x02) == 0x02)

        PBout(1) = 1;

    else
        PBout(1) = 0;

#ifdef  pcbxiao
    if ((dat & 0x04) == 0x04)

        PBout(13) = 1;

    else
        PBout(13) = 0;
#else
    if ((dat & 0x04) == 0x04)

        PBout(2) = 1;

    else
        PBout(2) = 0;
#endif
    if ((dat & 0x08) == 0x08)

        PBout(10) = 1;

    else
        PBout(10) = 0;

    if ((dat & 0x10) == 0x10)

        PBout(11) = 1;

    else
        PBout(11) = 0;

    if ((dat & 0x20) == 0x20)

        PBout(5) = 1;

    else
        PBout(5) = 0;
    if ((dat & 0x40) == 0x40)

        PBout(6) = 1;

    else
        PBout(6) = 0;

    if ((dat & 0x80) == 0x80)

        PBout(7) = 1;

    else
        PBout(7) = 0;


    delay_ms(1);
    PIN_LCD_ENa = 1;
    delay_ms(1);
    PIN_LCD_ENa = 0;
    delay_ms(1);
}

/***********************************************************************************************
函数名:	 往1602写数据
输入参数:dat:数据
输出:无
说明:无
***********************************************************************************************/
void LCDW_Dat(u8 dat)
{
    vu8 n = 0, s = 0,g=0;
    vu16 date = (GPIO_ReadOutputData(GPIOB) & 0xF300) + dat;
    PIN_LCD_RS = 1;
    delay_ms(1);


    if ((dat & 0x01) == 0x01)

        PBout(0) = 1;

    else
        PBout(0) = 0;
    if ((dat & 0x02) == 0x02)

        PBout(1) = 1;

    else
        PBout(1) = 0;
#ifdef  pcbxiao
    if ((dat & 0x04) == 0x04)

        PBout(13) = 1;

    else
        PBout(13) = 0;
#else
    if ((dat & 0x04) == 0x04)

        PBout(2) = 1;

    else
        PBout(2) = 0;
#endif
    if ((dat & 0x08) == 0x08)

        PBout(10) = 1;

    else
        PBout(10) = 0;

    if ((dat & 0x10) == 0x10)

        PBout(11) = 1;

    else
        PBout(11) = 0;

    if ((dat & 0x20) == 0x20)

        PBout(5) = 1;

    else
        PBout(5) = 0;
    if ((dat & 0x40) == 0x40)

        PBout(6) = 1;

    else
        PBout(6) = 0;

    if ((dat & 0x80) == 0x80)

        PBout(7) = 1;

    else
        PBout(7) = 0;


    delay_ms(1);
    PIN_LCD_ENa = 1;
    delay_ms(1);
    PIN_LCD_ENa = 0;
    delay_ms(1);
}
#else // 2024.9.4已经验证
#define PIN_LCD_RS1 PAout(11)
#define PIN_LCD_EN1 PAout(12)
#define out 0
#define od 1
#define in 2
#define PA 0
#define PB 1
#define PC 2
#define PD 3
extern void set(vu8 duankou, vu8 pin, vu8 mode);
void rw(vu8 com)
{
    if (com & 0x01)
        PAout(15) = 1;
    else
        PAout(15) = 0;

    if (com & 0x02)
        PBout(3) = 1;
    else
        PBout(3) = 0;

    if (com & 0x04)
        PBout(4) = 1;
    else
        PBout(4) = 0;

    if (com & 0x08)
        PBout(5) = 1;
    else
        PBout(5) = 0;

    if (com & 0x10)
        PBout(6) = 1;
    else
        PBout(6) = 0;

    if (com & 0x20)
        PBout(7) = 1;
    else
        PBout(7) = 0;

    if (com & 0x40)
        PBout(8) = 1;
    else
        PBout(8) = 0;

    if (com & 0x80)
        PBout(9) = 1;
    else
        PBout(9) = 0;
}
/***********************************************************************************************
函数名:	 往1602写命令
输入参数: com:命令值
输出:无
说明:无
***********************************************************************************************/
void LCDW_Com(u8 com)
{
    set(PA, 15, out);
    set(PA, 11, out);
    set(PA, 12, out);
    set(PB, 3, out);
    set(PB, 4, out);
    set(PB, 5, out);
    set(PB, 6, out);
    set(PB, 7, out);
    set(PB, 8, out);
    set(PB, 9, out);
    PIN_LCD_RS1 = 0;
    rw(com);
    PIN_LCD_EN1 = 1;
    PIN_LCD_EN1 = 0;
    delay_ms(2);
}

/***********************************************************************************************
函数名:	 往1602写数据
输入参数:dat:数据
输出:无
说明:无
***********************************************************************************************/
void LCDW_Dat(u8 dat)
{

    PIN_LCD_RS1 = 1;
    rw(dat);
    PIN_LCD_EN1 = 1;
    PIN_LCD_EN1 = 0;
    delay_ms(2);
}

#endif
/***********************************************************************************************
函数名:在指定的位置显示温度
输入参数:
 1.Line:行,取值0,1
 2.Row:列,取值0~15
 3.Dat:要显示的数据
输出:无
说明:无
***********************************************************************************************/
void LCD_DisStr(u8 Line, u8 Row, vu8 *Dat)
{
#ifdef lcd12864
    if (Line == 0)
        LCDW_Com(0x80 + Row);
    else if (Line == 1)
        LCDW_Com(0x90 + Row);
    if (Line == 2)
        LCDW_Com(0x88 + Row);
    if (Line == 3)
        LCDW_Com(0x98 + Row);
#else
    LCDW_Com(0x80 + Row + Line * 0x40); // 指定显示位置
#endif
    while (*Dat != '\0')
        LCDW_Dat(*Dat++);
}

void LCD_Dis2bit(vu8 Line, vu8 Row, vu8 *sa, vu8 value1)
{
    LCD_DisStr(Line, Row, sa);
    LCDW_Dat(value1 / 10 + 48);
    LCDW_Dat(value1 % 10 + 48);
}
void LCD_Dis3bit(vu8 Line, vu8 Row, vu8 *sa, vu16 value1)
{
    LCD_DisStr(Line, Row, sa);
    LCDW_Dat(value1 / 100 + 48);
    LCDW_Dat(value1 / 10 % 10 + 48);
    LCDW_Dat(value1 % 10 + 48);
}
extern vu8 ior(vu8 port1, vu8 bitt);
vu8 keyread(vu8 port, vu8 bitt)
{
    return ior(port, bitt); // 有按下按键返回低
}
extern void delayms(u16 nms);
vu8 keyread_xiaodou(vu8 port, vu8 bitt)
{
    if (ior(port, bitt) == 0) // 有按下按键
    {
        delayms(10);              // 延时函数,单位ms
        if (ior(port, bitt) == 0) // 再次判断有按下按键
        {
            delayms(10); // 延时函数,单位ms
            while (ior(port, bitt) == 0)
                ; // 等待松开
            return 0;
        }
        return 1;
    }
    return 1;
}
void LCD_Dis4bit(vu8 Line, vu8 Row, vu8 *sa, vu16 value1)
{
    LCD_DisStr(Line, Row, sa);
    LCDW_Dat(value1 / 1000 + 48);
    LCDW_Dat(value1 / 100 % 10 + 48);
    LCDW_Dat(value1 / 10 % 10 + 48);
    LCDW_Dat(value1 % 10 + 48);
}
void LCD_Dis5bit(vu8 Line, vu8 Row, vu8 *sa, vu32 value1)
{
    LCD_DisStr(Line, Row, sa);
    LCDW_Dat(value1 / 10000 + 48);
    LCDW_Dat(value1 / 1000 % 10 + 48);
    LCDW_Dat(value1 / 100 % 10 + 48);
    LCDW_Dat(value1 / 10 % 10 + 48);
    LCDW_Dat(value1 % 10 + 48);
}
void LCD_2bit(vu8 value1)
{
    LCDW_Dat(value1 / 10 + 48);
    LCDW_Dat(value1 % 10 + 48);
}
void LCD_2bit_vol(vu8 value1)
{

    LCDW_Dat(value1 / 10 + 48);
    LCDW_Dat('.');
    LCDW_Dat(value1 % 10 + 48);
}
void LCD_3bit(vu16 value1)
{
    LCDW_Dat(value1 / 100 + 48);
    LCDW_Dat(value1 / 10 % 10 + 48);
    LCDW_Dat(value1 % 10 + 48);
}
void LCD_3bit_ph(vu16 value1)
{

    LCDW_Dat(value1 / 100 + 48);
    LCDW_Dat(value1 / 10 % 10 + 48);
    LCDW_Dat('.');
    LCDW_Dat(value1 % 10 + 48);
}
void LCD_4bit(vu16 value1)
{
    LCDW_Dat(value1 / 1000 + 48);
    LCDW_Dat(value1 / 100 % 10 + 48);
    LCDW_Dat(value1 / 10 % 10 + 48);
    LCDW_Dat(value1 % 10 + 48);
}
void LCD_5bit(vu16 value1)
{
    LCDW_Dat(value1 / 10000 + 48);
    LCDW_Dat(value1 / 1000 % 10 + 48);
    LCDW_Dat(value1 / 100 % 10 + 48);
    LCDW_Dat(value1 / 10 % 10 + 48);
    LCDW_Dat(value1 % 10 + 48);
}

// 1602初始化
void LCD_Init(void)
{
    vu16 i;

    PIN_LED_Fac = 1;
    PIN_LED_Warn = 1;

    /*1602初始化*/
    LCDW_Com(0x38); // 00111000，设置16*2显示，5*7点阵，8位数据接口	LCDW_Com(0x0e);	//1110，开显示，显示光标，光标不闪烁
    LCDW_Com(0x0c); // 0110，当读或写一个字符后地址指针加1，光标加1
    LCDW_Com(0x06); // 0110，当读或写一个字符后地址指针加1，光标加1
    LCDW_Com(0x01); // 数据指针清零。清屏
    LCDW_Com(0x80); // 指针指向第一行的第一个字符
    LCDW_Com(0x01); // 清屏
}
