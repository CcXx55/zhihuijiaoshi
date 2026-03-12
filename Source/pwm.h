#define pwmmax 1
vu8 pwmzhi[pwmmax] = {0};
vu8 pwmcnt[pwmmax] = {0};

vu8 portpwm[pwmmax][2] = {PB, 15}; // 若写PA,12,1,代表设备接PA12口,高电平有效启动
void setpwm(vu8 zu, vu8 portt, vu8 bitt, vu8 pwmval)
{
    portpwm[zu][0] = portt;
    portpwm[zu][1] = bitt;
    pwmzhi[zu] = pwmval;
}

void pwm(void) // 软件pwm调风扇的速度
{
    vu8 i;

    for (i = 0; i < pwmmax; i++) // 有几个pwm就弄几次
    {
        if (pwmzhi[i] == 0)
        {

            iow(portpwm[i][0], portpwm[i][1], out, 0); // 输出低电平,不管设备高低电平有效,pwm都是先输出高
        }
        else if (pwmzhi[i] == 10)
            iow(portpwm[i][0], portpwm[i][1], out, 1); // 输出高电平
        else
        {
            if (pwmcnt[i] <= pwmzhi[i])                    // 如果变量小于指定值
                iow(portpwm[i][0], portpwm[i][1], out, 1); // 输出高电平,不管设备高低电平有效,pwm都是先输出高
            else if (pwmcnt[i] < 10)                       // 其他值
                iow(portpwm[i][0], portpwm[i][1], out, 0); // 输出低电平
            else
            {
                pwmcnt[i] = 0; // 清零变量
            }
            pwmcnt[i]++; // 变量加1
        }
    }
}