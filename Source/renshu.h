
#if 1 // 新版本
vu8 renshu = 0;                    // 定义人数


vu8 ina = 0, outa = 0, tia = 0;
void renshu1(void) // 人数函数
{
    vu8 i;
	if(ina)
		led2=0;//开灯指示
	else
		led2=1;//关灯
	if(outa)
		led1=0;//开灯指示
	else
		led1=1;	//关灯
    if (tia == 0) // 定时到
    {
  
        if ((ior(hw1_port, hw1_bit) == 0) && (ior(hw2_port, hw2_bit) == 1)) // 外面红外感应到人
        {
						tia = 5;                                                             // 500ms查询一次
            if (outa == 0) // 第一次感应
            {
                ina = 1; // 标记现在进来
            }
            else
            {
							outa=0;
                renshu++; // 人数+1
            }
        }
        else if ((ior(hw2_port, hw2_bit) == 0) && (ior(hw1_port, hw1_bit) == 1)) // 里面红外感应到人
        {
					      tia = 5;                                                             // 500ms查询一次
            if (ina == 0) // 第一次感应
            {
                outa = 1; // 标记出去
            }
            else
            {
							ina=0;
                // 如果条件满足
                if (renshu) // 人数-1
                    renshu--;
            }
        }
    }
    else
        tia--; // 时间减减
}


#else
void renshu1(void) // 人数函数
{
    vu8 i;
    if (ior(hw1_port, hw1_bit) == 0) // 外面红外感应到人
    {
        delayms(10);                   // 延时10ms
        while (ior(hw2_port, hw2_bit)) // 等等里面红外感应
            LCD_DisStr(0, 14, " A");   // 在指定位置显示A
        i = 0;                         // 清零参数
        while (1)
        {
            if (ior(hw2_port, hw2_bit)) // 里面红外没感应到人
                i++;                    // 变量累加
            else
                i = 0;    // 变量清零
            delayms(100); // 延时100ms
            if (i > 10)   // 100ms*10没感应人
                break;    // 就退出
        }
        renshu++; // 人数+1
    }
    if (ior(hw2_port, hw2_bit) == 0) // 里面红外感应到人
    {
        delayms(10);                   // 延时10ms
        while (ior(hw1_port, hw1_bit)) // 等等外面红外感应
            LCD_DisStr(0, 14, " B");
        i = 0; // 清零参数
        while (1)
        {
            if (ior(hw1_port, hw1_bit)) // 外面红外没感应到人
                i++;                    // 变量累加
            else
                i = 0;    // 变量清零
            delayms(100); // 延时100ms
            if (i > 10)   // 100ms*10没感应人
                break;    // 就退出
        }
        if (renshu) // 人数-1
            renshu--;
    }
    LCD_DisStr(0, 14, " ");
}
#endif