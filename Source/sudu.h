
vu16 sudu = 0;
vu8 oldpin = 255; // 速度变量
vu16 suduL;                 // 水流为低的变量
vu16 timesu = 0;            // 计时变量

void suducheck()            // 速度检测底层函数
{
    if (sudupin != oldpin) // 无论红外,输液,霍尔,电平都会呈现变化
    {
        suduL++;         // 变量+1
        oldpin = sudupin; // 记住当前的脚的电平
    }
    timesu++;          // 计时变量+1
    if (timesu > 999) // 计时变量>999,也就是1秒
    {
        timesu = 0;     // 清零
        sudu = suduL/4;   // 吧数据存入速度变量

        suduL = 0; // 清零
    }
}

