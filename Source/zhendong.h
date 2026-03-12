vu16 timeoutzd = 0; // 定义震动倒计时
vu8 oldzd = 255;	// 记录震动状态
vu8 cntzd = 0;

void zdcheck(void) // 定时器1ms调用,检测震动
{
	if (timeoutzd) // 倒计时不为0
	{
		timeoutzd--; // 倒计时减1
	}
	if (zd != oldzd) // 有震动
	{
		oldzd = zd;		// 保存震动状态
		cntzd++;		// 计数
		if (cntzd > 10) // 计数超过10
		{
			cntzd = 0;		  // 清零计数
			timeoutzd = 1000; // 定时时间设为1ms
		}
	}
}