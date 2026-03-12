/*常见问题

  1.左边main.c那个加号不用点开,这是一个程序模板,有调用的部分看这个main.c;

  2.请查看视频:链接：https://share.weiyun.com/fLbUW4rH,这些keil基本操作一定要学,
   有助于你更快掌握代码;

  3.因部分电脑配置不同,部分代码会显示红线,只要编译了,0个error就行了(有警告不管,不会影响运行,keil语法严格规定,
    机器又傻瓜式,不会去判断其实语法也可以那样写,自然给你警告);

  4.代码看懂方法,就是从下面的main(void)函数里面一行行的看,如果遇到不懂,复制去字典搜索,你可以上面第二点介绍
     方法去查看函数的具体代码,这些都是底层驱动,你如果没有基础或者熟读芯片手册,你是看不懂,我也不解释,老师有
     问,你就说买芯片厂家送的驱动代码或网上下,老师也都这鸟样;

     代码字典,打开这个链接：
     https://share.weiyun.com/tQL3ergu
     里面有教你C语言常用的语法,最主要就是那个代码字典,
     你如果代码不懂,就把代码扔到字典里面搜索,几乎涵盖大部分
*/
#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "include.h"
#include "1602.h" //如果用到OLED的同学吧这行删掉
#include "header.h"

#define jdqa_port1 PC
#define jdqa_bit1 15 // 若port=PA,bit=12,代表继电器脚接PA12,取值0模块指示灯亮

#define jdqa_port2 PA
#define jdqa_bit2 3 // 若port=PA,bit=12,代表继电器脚接PA12,取值0模块指示灯亮

#define duoji1_port PB
#define duoji1_bitt 14 // 若port=PA,bitt=11,代表舵机接PA11脚
#define duoji2_port PA
#define duoji2_bitt 15 // 若port=PA,bitt=11,代表舵机接PA11脚

#include "duoji.h"     //舵机的底层文件,若有>=2个舵机,去头文件内部改其他舵机IO


#define fs_port PB
#define fs_bit 15 // 若port=PA,bit=12,代表风扇脚接PA12

vu8 gm;            // 定义变量:光敏
#define gm_adcio 5 // 光敏模块,比如用PA0,就写0

#define beepa_porta PA
#define beepa_bita 12 // 若port=PA,bit=12,代表蜂鸣器脚接PA12,

// 如果STM32,模块V脚接IO口,模块IO脚接GND
#define dled_port PA
#define dled_bit 4 // 若port=PA,bit=12,代表单LED模块脚接PA12

#define hw1_port PC
#define hw1_bit 13                  // 若port=PA,bit=1,代表避障模块的D1脚接PA1
#define hw1 ior(hw1_port, hw1_bit) // 双路避障模块的D1脚,遮挡#1的红外,会输出低电平
#define hw2_port PC
#define hw2_bit 14                  // 若port=PA,bit=1,代表避障模块的D2脚接PA1
#define hw2 ior(hw2_port, hw2_bit) // 双路避障模块的D2脚,遮挡#2的红外,会输出低电平
#include "renshu.h"

vu8 mq135;            // 定义变量:空气质量粉尘PM25,烟雾MQ135
#define mq135_adcio 1 // mq135模块,比如AO接PA0,就写0

vu8 mq2;            // 定义变量:二氧化碳,烟雾MQ2
#define mq2_adcio 0 // mq2模块,比如AO接PA0,就写0

#include "18B20.h" //温度的底层文件
#define ds18B20_port PA
#define ds18B20_bit 11 // 若port=PA,bit=2,代表DS18B20脚接PA2

vu8 cn1 = 0;                  // 1ms计数变量
void TIM1_UP_IRQHandler(void) // 定时器是100us
{

    if (TIM == TIM1)
    {
        if (TIM_GetITStatus(TIM, TIM_IT_Update) != RESET) // 判断是否为 更新事件 标志位
        {
            TIM_ClearITPendingBit(TIM, TIM_IT_Update); // 清除 	更新事件 标志
            uart_xuni();                               // 虚拟串口
            cn1++;                                     // 变量+1
            if (cn1 > 9)                               // 100us*10=1ms
            {
                cn1 = 0;       // 清零变量
                uartinitirq(); // 负责接收串口
            }
        }
    }
}

int main(void)
{
    vu16 i, j = 0, k = 0, flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag5 = 0, flag6 = 0, flag7 = 0; // jdq
    RCC_Configuration();                                                                               // 时钟初始化
    NVIC_Configuration();                                                                              // 中断初始化 sendchar
    delay_init(72);                                                                                    // 延时初始化
    delay_ms(300);                                                                                     // 延时300ms,因为要禁用jtag口
    // 打开PA PB PC口的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    // 关闭JTAG,不然液晶不能正常显示
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIOAllInit(); // IO口全部初始化
    SysRead();     // 读取系统的参数

    UARTT_Init(USART1, 9600); // 串口初始化

    Tim1_Init(1); // 定时器初始化

    GPIOO_Init(); // IO口初始化,包括板子自带的按键和灯

    UARTT_Init(USART2, 9600); // 串口初始化
    // 串口4若接收不了,则修改TIM1_XinInit()函数里面定时,原本3744,现在改3844,
    // 矩阵,PM25，hx711,max30100都可以（因为中微函数自己写,延时不准）,
    // 若在遇不准可微调3844

    UART4_INIT();    // sendstr(4,"152");接收函数vscode搜ckr4就行  TX4  发送脚
    set(PA, 7, out); // OLED供电

    //  UARTT_Init(USART3,9600); //C6T6没串口3,	Tx引脚,1602_D3脚,Rx=D4脚,必须放while前面
    while (1)
    {

        /*
        温度低了，开空调，高了开风扇/空调;光照暗，开灯，光照强，拉窗帘;
        室内无人关灯;空气质量差，开窗，等;增加手机APP远程查看并控制
        可以在这基础上再增加一些功能，也可以加烟雾报警传感器，还有人数统计功能
        窗帘,窗户2个舵机,空调2个继电器,冷热，没人用人数统计去算
        */

        // 演示时候说一只手不好操作,你就说点燃烟雾,靠近,它的值会变大
        mq135 = GetADCResult(mq135_adcio, 1); // 获取MQ135值,3代表P13(12C)或PA3(STM32),1代表要百分比
                                              // 原理:双路避障2个红外,#1的放在门外,#2放门内,如果先#1感应到,屏幕显示A,然后#2再
        // 感应到,相当于人从门外走到里面,人数+1,否则先#2感应到,屏幕显示B,然后#1再
        // 感应到,相当于人从门内走到外面,人数-1
        renshu1();                                  // 读取人数,值放在renshu变量
        gm = 98 - GetADCResult(gm_adcio, 1);        // 获取光敏值,值越小越暗,3代表P13(12C,51不行)或PA3(STM32),1代表要百分比
                                                    // 演示时候说一只手不好操作,你就说点燃烟雾,靠近,它的值会变大
        mq2 = GetADCResult(mq2_adcio, 1);           // 获取MQ2值,3代表P13(12C)或PA3(STM32),1代表要百分比
                                                    // PA,11代表温度接在PA11,内部已初始化io口为od模式
        DS18B20_GetTemp(ds18B20_port, ds18B20_bit); // 温度值存变量ds18b20temp,演示时候,要手摸让温度上升给客户看

        morepage_oled(renshu, mq135, gm, mq2, ds18b20temp,flag1); // 显示4参数值(数据与数据用空格隔开,不显示就填0)及对应设置值,按键操作

        if (flag1) // 手动模式
        {
            // 窗户手动控制
            if (flag2)
            {
                duoji(0, ON); // 打开舵机1
            }
            else
            {
                duoji(0, OFF); // 关闭舵机1
            }
            // 窗帘手动控制
            if (flag3)
            {
                duoji(1, ON); // 打开舵机1
            }
            else
            {
                duoji(1, OFF); // 关闭舵机1
            }
            // 空调手动控制
            if (flag4)
            {
                iow(jdqa_port1, jdqa_bit1, out, 0); // 打开继电器
                iow(jdqa_port2, jdqa_bit2, out, 1); // 关闭继电器
						
            }
            else if (flag5)
            {
                iow(jdqa_port2, jdqa_bit2, out, 0); // 打开继电器
                iow(jdqa_port1, jdqa_bit1, out, 1); // 关闭继电器
						
            }
            else
            {
                iow(jdqa_port1, jdqa_bit1, out, 1); // 关闭继电器
                iow(jdqa_port2, jdqa_bit2, out, 1); // 关闭继电器
            }
            // 风扇控制
            if (flag6)
            {
                iow(fs_port, fs_bit, out, 1); // 打开风扇
            }
            else
            {
                iow(fs_port, fs_bit, out, 0); // 关闭风扇
            }
            // 灯光控制
            if (flag7)
            {
                iow(dled_port, dled_bit, out, 1); // led亮
            }
            else
            {
                iow(dled_port, dled_bit, out, 0); // led灭
            }
        }
        else // 自动模式
        {
            // 温度过低，开空调制热，温度过高开制冷空调，
            if (ds18b20temp < sys[0])
            {
                iow(jdqa_port2, jdqa_bit2, out, 0); // 打开空调制热
                iow(jdqa_port1, jdqa_bit1, out, 1); // 关闭制冷空调
                iow(fs_port, fs_bit, out, 0);       // 关闭风扇
            }
            else if (ds18b20temp > sys[1])
            {
                iow(jdqa_port1, jdqa_bit1, out, 0); // 打开制冷空调
                iow(jdqa_port2, jdqa_bit2, out, 1); // 关闭空调制热
                iow(fs_port, fs_bit, out, 1);       // 关闭风扇
            }
            else
            {
                iow(jdqa_port1, jdqa_bit1, out, 1); // 关闭制冷空调
                iow(jdqa_port2, jdqa_bit2, out, 1); // 关闭空调制热
                iow(fs_port, fs_bit, out, 0);       // 打开风扇
            }

            // 灯光自动控制
            if ((renshu > 0) && (gm < sys[2])) // 有人且光照暗
            {
                iow(dled_port, dled_bit, out, 1); // led亮
            }
            else
            {
                iow(dled_port, dled_bit, out, 0); // led灭
            }

            // 光照强，拉窗帘
            if (gm > sys[3])
            {
                duoji(1, ON); // 打开舵机1
            }
            else
            {
                duoji(1, OFF); // 关闭舵机1
            }

            // 空气质量差，开窗
            if (mq135 > sys[5])
            {
                duoji(0, ON); // 打开舵机1
            }
            else
            {
                duoji(0, OFF); // 关闭舵机1
            }

            // 烟雾报警
            if (mq2 > sys[4])
            {
                iow_qufan(beepa_porta, beepa_bita, out); // 蜂鸣器滴滴
            }
            else
            {
                iow(beepa_porta, beepa_bita, out, 1); // 关闭蜂鸣器
            }
        }

        if (Uart_RecOk) // 如果收到wifi数据
        {
            Uart_RecOk = 0; // 清除标志

            sys[0] = strtohex(&Uart_Rx1[0], "k1", &sys[0]); // 比如dat11的值为字符串123,则strtohex函数转成十进制123存入到变量
            sys[1] = strtohex(&Uart_Rx1[0], "k2", &sys[1]); // 比如dat11的值为字符串123,则strtohex函数转成十进制123存入到变量
            sys[2] = strtohex(&Uart_Rx1[0], "k3", &sys[2]); // 比如dat11的值为字符串123,则strtohex函数转成十进制123存入到变量
            sys[3] = strtohex(&Uart_Rx1[0], "k4", &sys[3]); // 比如dat11的值为字符串123,则strtohex函数转成十进制123存入到变量
            sys[4] = strtohex(&Uart_Rx1[0], "k5", &sys[4]); // 比如dat11的值为字符串123,则strtohex函数转成十进制123存入到变量
            sys[5] = strtohex(&Uart_Rx1[0], "k6", &sys[5]); // 比如dat11的值为字符串123,则strtohex函数转成十进制123存入到变量
            SysSave();                                      // 保存参数

            if (strstr((const char *)&Uart_Rx1[0], "ka")) // 收到指定的命令,就是app代码里面的k1,k2函数....
            {

                flag1 = 0; // 手动/自动模式切换
            }
            if (strstr((const char *)&Uart_Rx1[0], "kb")) // 收到指定的命令,就是app代码里面的k1,k2函数....
            {
                flag1 = 1;
                flag2 = 1 - flag2; // 窗户开关
            }
            if (strstr((const char *)&Uart_Rx1[0], "kc")) // 收到指定的命令,就是app代码里面的k1,k2函数....
            {
                flag1 = 1;
                flag3 = 1 - flag3; // 窗帘开关
            }
            if (strstr((const char *)&Uart_Rx1[0], "kd")) // 收到指定的命令,就是app代码里面的k1,k2函数....
            {
                flag1 = 1;
                flag4 = 1 - flag4; // 冷空调开关
                if (flag4)         // 如果打开制冷
                {
                    flag5 = 0; // 强制关闭制热
                }
            }
            if (strstr((const char *)&Uart_Rx1[0], "ke")) // 收到指定的命令,就是app代码里面的k1,k2函数....
            {
                flag1 = 1;
                flag5 = 1 - flag5; // 热空调开关
                if (flag5)         // 如果打开制热
                {
                    flag4 = 0; // 强制关闭制冷
                }
            }
            if (strstr((const char *)&Uart_Rx1[0], "kf")) // 收到指定的命令,就是app代码里面的k1,k2函数....
            {
                flag1 = 1;
                flag6 = 1 - flag6; // 风扇开关
            }
            if (strstr((const char *)&Uart_Rx1[0], "kg")) // 收到指定的命令,就是app代码里面的k1,k2函数....
            {
                flag1 = 1;
                flag7 = 1 - flag7; // 灯光开关
            }

            for (i = 0; i < 30; i++) // 循环指定次数
            {
                Uart_Rx1[i] = 0;
            }
        }

        k++;        // 变量+1
        if (k > 12) // 变量>12,也就12*100ms=1.2秒
        {
            k = 0;                          // 清零变量
            wifi_start(uartwifi);           // 启动WIFI
            wifi_sendnum(uartwifi, renshu); // 发送数据1   TempNow
            wifi_sendnum(uartwifi, mq135);  // 发送数据2
            wifi_sendnum(uartwifi, gm);     // 发送数据1   TempNow
            wifi_sendnum(uartwifi, mq2);    // 发送数据2
            wifi_sendnum(uartwifi, flag1);  // 发送数据1
					      wifi_sendnum(uartwifi,ds18b20temp);  // 发送数据1
            wifi_end(uartwifi, "121801");   // 结束WIFI并真正发送,后面字符串就是WIFI号码
        }

        delayms(100); // 延时100ms
    }
}
