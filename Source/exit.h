/////////////////////////////////直接复制代码,库文件添加就可以
#define PORTA    0
#define PORTB    1
#define PORTC    2
#define PORTD    3
#define PORTE    4
#define PORTF    5
#define PORTG    6

#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6
#define PIN7    7
#define PIN8    8
#define PIN9    9
#define PIN10    10
#define PIN11    11
#define PIN12    12
#define PIN13    13
#define PIN14    14
#define PIN15    15
#define BYTE vu8
#define BOOL vu8

GPIO_TypeDef* GPIO_PORT[] =
{
    GPIOA, GPIOB, GPIOC,
    GPIOD, GPIOE, GPIOF, GPIOG,
};
const u16 GPIO_Pin[]=
{
    GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3,
    GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7,
    GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10,GPIO_Pin_11,
    GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15,
};
const u32 GPIO_PortClk[] =
{
    RCC_APB2Periph_GPIOA,
    RCC_APB2Periph_GPIOB,
    RCC_APB2Periph_GPIOC,
    RCC_APB2Periph_GPIOD,
    RCC_APB2Periph_GPIOE,
    RCC_APB2Periph_GPIOF,
    RCC_APB2Periph_GPIOG,
};
const u8 GPIO_PortSuorce[] =
{
    GPIO_PortSourceGPIOA,
    GPIO_PortSourceGPIOB,
    GPIO_PortSourceGPIOC,
    GPIO_PortSourceGPIOD,
    GPIO_PortSourceGPIOE,
    GPIO_PortSourceGPIOF,
    GPIO_PortSourceGPIOG,
};
const u8 GPIO_PinSource[] =
{
    GPIO_PinSource0, GPIO_PinSource1, GPIO_PinSource2, GPIO_PinSource3,
    GPIO_PinSource4, GPIO_PinSource5, GPIO_PinSource6, GPIO_PinSource7,
    GPIO_PinSource8, GPIO_PinSource9, GPIO_PinSource10,GPIO_PinSource11,
    GPIO_PinSource12,GPIO_PinSource13,GPIO_PinSource14,GPIO_PinSource15,
};
const u32 EXINT_Line[] =
{
    EXTI_Line0, EXTI_Line1, EXTI_Line2, EXTI_Line3,
    EXTI_Line4, EXTI_Line5, EXTI_Line6, EXTI_Line7,
    EXTI_Line8, EXTI_Line9, EXTI_Line10,EXTI_Line11,
    EXTI_Line12,EXTI_Line13,EXTI_Line14,EXTI_Line15,
};
typedef enum IRQn
{
    /******  STM32 specific Interrupt Numbers *********************************************************/
    WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
    PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
    TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
    RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
    FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                               */
    RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                 */
    EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
    EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
    EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
    EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
    EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                 */
    DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                      */
    DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                      */
    DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                      */
    DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                      */
    DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                      */
    DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                      */
    DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                      */

    ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
    CAN1_TX_IRQn                = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
    CAN1_RX0_IRQn               = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
    CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
    CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
    EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
    TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
    TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
    TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
    TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
    TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
    TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
    TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
    I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
    I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
    I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
    I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
    SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
    SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
    USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
    USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
    USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
    EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
    RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
    OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS WakeUp from suspend through EXTI Line Interrupt */
    TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                */
    SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                */
    UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                               */
    UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                               */
    TIM6_IRQn                   = 54,     /*!< TIM6 global Interrupt                                */
    TIM7_IRQn                   = 55,     /*!< TIM7 global Interrupt                                */
    DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                      */
    DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                      */
    DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                      */
    DMA2_Channel4_IRQn          = 59,     /*!< DMA2 Channel 4 global Interrupt                      */
    DMA2_Channel5_IRQn          = 60,     /*!< DMA2 Channel 5 global Interrupt                      */
    ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                            */
    ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt          */
    CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                    */
    CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                   */
    CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                   */
    CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                   */
    OTG_FS_IRQn                 = 67      /*!< USB OTG FS global Interrupt                          */

} IRQn_Type;
const IRQn_Type Exit_IRQn[] =
{
    EXTI0_IRQn,          // EXTI Line0 Interrupt
    EXTI1_IRQn,         // EXTI Line1 Interrupt
    EXTI2_IRQn,         // EXTI Line2 Interrupt
    EXTI3_IRQn,         // EXTI Line3 Interrupt
    EXTI4_IRQn,         // EXTI Line4 Interrupt
    EXTI9_5_IRQn,        // 5
    EXTI9_5_IRQn,        // 6
    EXTI9_5_IRQn,        // 7
    EXTI9_5_IRQn,        // 8
    EXTI9_5_IRQn,       // External Line[9:5] Interrupts
    EXTI15_10_IRQn,        // 10
    EXTI15_10_IRQn,        // 11
    EXTI15_10_IRQn,        // 12
    EXTI15_10_IRQn,        // 13
    EXTI15_10_IRQn,        // 14
    EXTI15_10_IRQn,     // External Line[15:10] Interrupts
};
#define uint32_t vu32
//系统中断管理
void NVIC_Config(uint32_t NVIC_PriorityGroup)
{
    //设置优先级分组：先占优先级0位,从优先级4位,芯片复位后默认为分组0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup);
    //设置向量表的位置和偏移,芯片复位后向量表默认在Flah中
#ifdef  VECT_TAB_RAM
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);     //向量表位于RAM
#else  /* VECT_TAB_FLASH  */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   //向量表位于FLASH
#endif
}

static u8 GetGpioPortNumber(GPIO_TypeDef* gpio_port)
{
    u8 i = 0;
    //参数检查
    assert_param(IS_GPIO_PIN(gpio_port));
    for(i = 0; i < 6; i ++)
    {
        if(gpio_port == GPIO_PORT[i])
        {
            return i;
        }
    }
    return 0;                                    //不会执行到这里
}
static u8 GetGpioPinNumber(u16 gpio_pin)
{
    u8 i = 0;
    //参数检查
    assert_param(IS_GPIO_ALL_PERIPH(gpio_pin));
    for(i = 0; i < 16; i ++)
    {
        if(gpio_pin == GPIO_Pin[i])
        {
            return i;
        }
    }
    return 0;                                    //不会执行到这里
}
/*************************************************
gpio_pin_mode:
    GPIO_Mode_AIN                 模拟输入
    GPIO_Mode_IN_FLOATING         浮空输入
    GPIO_Mode_IPD                 下拉输入
    GPIO_Mode_IPU                 上拉输入

    GPIO_Mode_Out_OD             开漏输出
    GPIO_Mode_Out_PP             推挽输出
    GPIO_Mode_AF_OD                复用开漏输出
    GPIO_Mode_AF_PP             复用推挽输出
gpio_pin_speed：
    0                            输入模式
    GPIO_Speed_10MHz
      GPIO_Speed_2MHz,
      GPIO_Speed_50MHz
调用形式:
GpioInit(GPIOA, GPIO_Pin_1, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
*************************************************/
void GpioInit(GPIO_TypeDef* gpio_port,
              u16 gpio_pin,
              GPIOMode_TypeDef gpio_pin_mode,
              GPIOSpeed_TypeDef gpio_pin_speed)
{
    u8 gpio_port_num = 0;
    GPIO_InitTypeDef GPIO_InitStructure;

    gpio_port_num = GetGpioPortNumber(gpio_port);

    RCC_APB2PeriphClockCmd(GPIO_PortClk[gpio_port_num], ENABLE);//使能时钟

    GPIO_InitStructure.GPIO_Mode = gpio_pin_mode;
    GPIO_InitStructure.GPIO_Pin = gpio_pin;
    if(gpio_pin_speed)
    {   //在输入模式下,该域不起作用
        GPIO_InitStructure.GPIO_Speed = gpio_pin_speed;
    }
    GPIO_Init(gpio_port, &GPIO_InitStructure);    //初始化端口
}
/*************************************************
gpio_pin_mode:
    GPIO_Mode_IN_FLOATING         浮空输入
    GPIO_Mode_IPD                 下拉输入
    GPIO_Mode_IPU                 上拉输入
exit_type:
    EXTI_Trigger_Rising         上升沿
    EXTI_Trigger_Falling        下降沿
    EXTI_Trigger_Rising_Falling  跳变沿(上升和下降)
调用形式:
Exit_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_IPU, EXTI_Trigger_Falling, 2, 3);
*************************************************/

void Exit_Init(GPIO_TypeDef* gpio_port,
               u16 gpio_pin,
               GPIOMode_TypeDef gpio_pin_mode,
               EXTITrigger_TypeDef exit_type,
               u8 pre_pri,
               u8 sub_pri)
{

    u8 gpio_port_num = 0;
    u8 gpio_pin_num = 0;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    gpio_port_num = GetGpioPortNumber(gpio_port);
    gpio_pin_num = GetGpioPinNumber(gpio_pin);
    //引脚初始化
    GpioInit(gpio_port, gpio_pin, gpio_pin_mode, (GPIOSpeed_TypeDef)0);
    //使能辅助时钟AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    ///////////////////////////////////////
    //将中断IO引脚映射到中断源(EXTI Line)
    GPIO_EXTILineConfig(GPIO_PortSuorce[gpio_port_num], GPIO_PinSource[gpio_pin_num]);
    //配置中断线
    EXTI_InitStructure.EXTI_Line = EXINT_Line[gpio_pin_num];
    //配置为外中断
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    //设置触发沿
    EXTI_InitStructure.EXTI_Trigger = exit_type;
    //使能中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    ///////////////////////////////////////
    //配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = Exit_IRQn[gpio_pin_num];    //引脚对应的中断IRQn

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre_pri;    //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_pri;        //响应优先级

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                    //使能IRQn

    NVIC_Init(&NVIC_InitStructure);
}
BYTE  RDAT4=0, RBIT4=0;
BOOL startrec4=0;
vu8 RecCnt4 = 0;
vu8 xuni_timeout4=0,Uart_RecOk4=0;
vu8 Uart_Rx4[100];

BYTE  RDAT5=0, RBIT5=0;
BOOL startrec5=0;
vu8 RecCnt5 = 0;
vu8 xuni_timeout5=0,Uart_RecOk5=0;
vu8 Uart_Rx5[100];

extern void set( vu8 duankou,vu8 pin,vu8 mode);
#define out 0
#define od 1
#define in 2
#define PA 0
#define PB 1
#define PC 2
#define PD 3

void TIM1_XinInit(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    vu16 TIM_COUNTER=1;
    vu16 TIM_PSC=3844;					  //3744就是104us,不能100us,不准
    /* TIM时钟使能 */

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* 基定时器初始化 */
    TIM_TimeBaseStructure.TIM_Period = TIM_COUNTER;				//计数值
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_PSC;				//分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//时钟分割：寄存器（技术）手册基定时器未讲到。
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//重复计数值

    TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);				//初始化TIM

}
