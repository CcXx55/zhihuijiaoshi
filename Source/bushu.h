	vu8 bushucnt = 0, timebu = 0; // 步数变量
void bushucheck(void) // 步数函数
{
	if (pinbushu == 0) // 如果有步数
	{
		if (timebu == 0) // 如果步数时间倒计时为0
			bushucnt++;	 // 步数变量+1
		timebu = 10;	 // 10*100ms=1秒,定义步数
	}
	else
	{
		if (timebu) // 如果步数时间倒计时不为0,就进行减减
			timebu--;
	}
}