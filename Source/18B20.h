/**
vu16 a=DS18B20_GetTemp();a为得到的温度,带有小数点,内部带自动完成初始化
**/
// 修改IO口下面3行都要修改,可以修改这3变量值,以支持多路的DHT11(看下方演示)
u32 ds18b20output_addr = GPIOA_ODR_Addr; // 代表用PA口,ODR代表输出
u32 ds18b20input_addr = GPIOA_IDR_Addr;  // 代表用PA口,IDR代表输入
vu8 ds18b20_bit = 11;                    // 无论输入输出都是11口
vu8 ds18b20temp;
void DS18B20_IO_IN(void)
{
    vu8 duankou = 0;
    if (ds18b20output_addr == GPIOA_ODR_Addr)
        duankou = 0;
    if (ds18b20output_addr == GPIOB_ODR_Addr)
        duankou = 1;
    if (ds18b20output_addr == GPIOC_ODR_Addr)
        duankou = 2;
    set(duankou, ds18b20_bit, in);
} // 代表用PA11口
void DS18B20_IO_OUT(void)

{
    vu8 duankou = 0;
    if (ds18b20output_addr == GPIOA_ODR_Addr)
        duankou = 0;
    if (ds18b20output_addr == GPIOB_ODR_Addr)
        duankou = 1;
    if (ds18b20output_addr == GPIOC_ODR_Addr)
        duankou = 2;
    set(duankou, ds18b20_bit, out);

} // 代表用PA11口
// 复位DS18B20
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT();                              // SET PG11 OUTPUT
    BIT_ADDR(ds18b20output_addr, ds18b20_bit) = 0; // 拉低DQ
    delay_us(750);                                 // 拉低750us
    BIT_ADDR(ds18b20output_addr, ds18b20_bit) = 1; // DQ=1
    delay_us(15);                                  // 15US
}
// 等待DS18B20的回应
// 返回1:未检测到DS18B20的存在
// 返回0:存在
u8 DS18B20_Check(void)
{
    u8 retry = 0;
    DS18B20_IO_IN(); // SET PG11 INPUT
    while (BIT_ADDR(ds18b20input_addr, ds18b20_bit) && retry < 200)
    {
        retry++;
        delay_us(1);
    };
    if (retry >= 200)
        return 1;
    else
        retry = 0;
    while (!BIT_ADDR(ds18b20input_addr, ds18b20_bit) && retry < 240)
    {
        retry++;
        delay_us(1);
    };
    if (retry >= 240)
        return 1;
    return 0;
}
// 从DS18B20读取一个位
// 返回值：1/0
u8 DS18B20_Read_Bit(void) // read one bit
{
    u8 data;
    DS18B20_IO_OUT(); // SET PG11 OUTPUT
    BIT_ADDR(ds18b20output_addr, ds18b20_bit) = 0;
    delay_us(2);
    BIT_ADDR(ds18b20output_addr, ds18b20_bit) = 1;
    DS18B20_IO_IN(); // SET PG11 INPUT
    delay_us(12);
    if (BIT_ADDR(ds18b20input_addr, ds18b20_bit))
        data = 1;
    else
        data = 0;
    delay_us(50);
    return data;
}
// 从DS18B20读取一个字节
// 返回值：读到的数据
u8 DS18B20_Read_Byte(void) // read one byte
{
    u8 i, j, dat;
    dat = 0;
    for (i = 1; i <= 8; i++)
    {
        j = DS18B20_Read_Bit();
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}
// 写一个字节到DS18B20
// dat：要写入的字节
void DS18B20_Write_Byte(u8 dat)
{
    u8 j;
    u8 testb;
    DS18B20_IO_OUT(); // SET PG11 OUTPUT;
    for (j = 1; j <= 8; j++)
    {
        testb = dat & 0x01;
        dat = dat >> 1;
        if (testb)
        {
            BIT_ADDR(ds18b20output_addr, ds18b20_bit) = 0; // Write 1
            delay_us(2);
            BIT_ADDR(ds18b20output_addr, ds18b20_bit) = 1;
            delay_us(60);
        }
        else
        {
            BIT_ADDR(ds18b20output_addr, ds18b20_bit) = 0; // Write 0
            delay_us(60);
            BIT_ADDR(ds18b20output_addr, ds18b20_bit) = 1;
            delay_us(2);
        }
    }
}
// 开始温度转换
void DS18B20_Start(void) // ds1820 start convert
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc); // skip rom
    DS18B20_Write_Byte(0x44); // convert
}
// 初始化DS18B20的IO口 DQ 同时检测DS的存在
// 返回1:不存在
// 返回0:存在
u8 DS18B20_Init(void)
{

    DS18B20_Rst();
    return DS18B20_Check();
}
vu8 fkag = 0;
vu16 old=255;
// 从ds18b20得到温度值
// 精度：0.1C
// 返回值：温度值 （-550~1250）
void DS18B20_GetTemp(vu8 port, vu8 bit1)
{
    u8 temp1,i;
    u8 TL, TH;
    short tem;
    set(port, bit1, od);
    if (port == PA)
    {

        ds18b20output_addr = GPIOA_ODR_Addr; // 代表用PA口,ODR代表输出
        ds18b20input_addr = GPIOA_IDR_Addr;  // 代表用PA口,ODR代表输出
    }
    if (port == PB)
    {

        ds18b20output_addr = GPIOB_ODR_Addr; // 代表用PA口,ODR代表输出
        ds18b20input_addr = GPIOB_IDR_Addr;  // 代表用PA口,ODR代表输出
    }
    if (port == PC)
    {

        ds18b20output_addr = GPIOC_ODR_Addr; // 代表用PA口,ODR代表输出
        ds18b20input_addr = GPIOC_IDR_Addr;  // 代表用PA口,ODR代表输出
    }

    ds18b20_bit = bit1;        // 比如bit=4,代表用PA4
    if (fkag == 0)
    {
        fkag = temp1;
        fkag = 1;
        DS18B20_Init();
        delayms(30);
    }
    DS18B20_Start(); // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc); // skip rom
    DS18B20_Write_Byte(0xbe); // convert
    TL = DS18B20_Read_Byte(); // LSB
    TH = DS18B20_Read_Byte(); // MSB

    if (TH > 7)
    {
        TH = ~TH;
        TL = ~TL;
        temp1 = 0; // 温度为负
    }
    else
        temp1 = 1; // 温度为正
    tem = TH;      // 获得高八位
    tem <<= 8;
    tem += TL;                 // 获得底八位
    tem = (float)tem * 0.0625; // 转换
 if (tem < 85)
        {
            if(old==255)
                old=tem;
            if(old<tem)
                i=tem-old;
            else
                i=old-tem;
            if(i<=2)
            {
                ds18b20temp = tem;
                    old=tem;
            }
    //    if(temp)return tem; //返回温度值
    //    else return -tem;
            
        }
    //	if(temp)return tem; //返回温度值
    //	else return -tem;
}
