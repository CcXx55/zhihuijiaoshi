
vu16 flagshengyin = 0; // 定义声音倒计时
vu16 cntsheng = 0;     // 定义变量

void voicecheck(void) // 定时器1ms调用
{
    if (flagshengyin) // 声音倒计时,每调用本函数,就被减1
        flagshengyin--;
    if (shengyin == 1) // 如果有声音
    {
        cntsheng++;        // 变量+1
        if (cntsheng > 15) // 超过15ms有声音
        {
            flagshengyin = 666; // 标志标志位,666代表666ms,因为声音是瞬间出现,为了给程序有时间处理
            cntsheng = 0;       // 清零变量
        }
    }
    else
    {
        cntsheng = 0; // 清零变量
    }
}