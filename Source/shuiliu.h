// 水流跟风速一样,1升就是空气的1立方米,1升5400转
vu8 shuiliusudu = 0; // 水流或风速速度,试过最大255,单位为mL/s或者cm3/s(立方厘米,风速)
vu16 shuiliusum = 0; // 水流或风速总的流量,单位mL或者cm3(立方厘米,风速)
#define pin_shuiliu PBin(15)
vu16 slsu = 0;
vu8 shuiliuold = 255;
vu16 shuiliuti = 0;
				

// 定时器1ms调用一次
void shuiliu(void)
{
    shuiliuti++;//累积
    if (shuiliuti > 999) // 1s秒
    {
        shuiliuti = 0;
        shuiliusudu = 1000.0 / 5400.0 * slsu;//1000毫升5400转
        shuiliusum = shuiliusum + shuiliusudu;//累加每秒速度
        slsu = 0;//清零速度
    }
}

void EXTI15_10_IRQHandler(void) // 外部中断,选择0到4,还有
{
    if (EXTI_GetITStatus(EXTI_Line15) != RESET) // 检查指定的EXTI0线路触发请求发生与否
    {
        EXTI_ClearITPendingBit(EXTI_Line15); // 清除EXTI0线路挂起位
        if ((pin_shuiliu != shuiliuold))    // 如果发生电平改变

        {
            shuiliuold = pin_shuiliu;//记住最新电平
            slsu++;//变量+1
        }
    }
}