// Measure_Distance(vu8 hang,vu8 lie),内部已经完成初始化
// 距离显示,然后distance为值,比如23.1cm,则distance=231mm
u32 output_addr;
vu8 output_bit;
u32 input_addr;
vu8 input_bit;

/************************************宏定义************************************/
#define VELOCITY_30C 3495 // 30摄氏度时的声速，声速V= 331.5 + 0.6*温度；
#define VELOCITY_23C 3453 // 23摄氏度时的声速，声速V= 331.5 + 0.6*温度；
/************************************位定义************************************/
unsigned long int distance = 0;

/******************************************************************************/
/* 函数名称  : Trig_SuperSonic                                                */
/* 函数描述  : 发出声波函数                                                   */
/* 输入参数  : 无                                                             */
/* 参数描述  : 无                                                             */
/* 返回值    : 无                                                             */
/******************************************************************************/
void Trig_SuperSonic(void) // 出发声波
{
    vu8 duankou;
    // IO口初始化
    if (output_addr == GPIOA_ODR_Addr)
        duankou = 0;
    if (output_addr == GPIOB_ODR_Addr)
        duankou = 1;
    if (output_addr == GPIOC_ODR_Addr)
        duankou = 2;
    set(duankou, output_bit, out);

    if (input_addr == GPIOA_ODR_Addr)
        duankou = 0;
    if (input_addr == GPIOB_ODR_Addr)
        duankou = 1;
    if (input_addr == GPIOC_ODR_Addr)
        duankou = 2;

    BIT_ADDR(output_addr, output_bit) = 1;
    delay_us(30);
    BIT_ADDR(output_addr, output_bit) = 0;
}

/******************************************************************************/
/* 函数名称  : Measure_Distance                                               */
/* 函数描述  : 计算距离函数                                                   */
// distance为值,比如23.1cm,则distance=231mm ,flag=1,则显示                                                       */
/******************************************************************************/

// 调用:Measure_Distance(PA,4,PA,5);//ODR代表输出,所以TRIG脚PA4,IDR输入,ECHO脚PA5
void Measure_Distance(vu8 outport, vu8 outbit, vu8 inputport, vu8 inputbit)
{
    vu32 i = 0;

    vu8 static ka = 0;
    ka++;
    set(outport, outbit, out);
    set(inputport, inputbit, in);
    if (outport == PA)

        output_addr = GPIOA_ODR_Addr; // 代表用PA口,ODR代表输出

    if (outport == PB)

        output_addr = GPIOB_ODR_Addr; // 代表用PA口,ODR代表输出

    if (outport == PC)

        output_addr = GPIOC_ODR_Addr; // 代表用PA口,ODR代表输出
    if (inputport == PA)

        input_addr = GPIOA_IDR_Addr; // 代表用PA口,ODR代表输出

    if (inputport == PB)

        input_addr = GPIOB_IDR_Addr; // 代表用PA口,ODR代表输出

    if (inputport == PC)

        input_addr = GPIOC_IDR_Addr; // 代表用PA口,ODR代表输出

    output_bit = outbit;
    input_bit = inputbit;
    if (1) // 延时，两次发射之间要至少有10ms间隔
    {
        ka = 0;
        Trig_SuperSonic(); // 触发超声波发射
        while (BIT_ADDR(input_addr, input_bit) == 0)
            ; // 等待回声

        while (BIT_ADDR(input_addr, input_bit))
        {

            delay_us(1);
            i++;
            if (i > 18000)
                break;
        }

        distance = VELOCITY_30C * i / 20000 * 2; //   1cm/s=1/1000 cm/ms

        distance /= 10;
    }
}

/******************************************************************************/
