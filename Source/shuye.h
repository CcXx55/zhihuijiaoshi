
vu16 shuyesudu=0;
vu16 shuyecnt=255,shuyeold=255;
vu16 shuyetime=0;
vu8 shuyeti=0;
#define hw PAin(0)

//定时器100us调用一次
void shuye_timer1(void)
{


    if(shuyeti)//就是滴数消抖,在倒计时结束前,再次检测外部中断,计数也不会加
        shuyeti--;
    shuyetime++;//计算2滴之间的时间间隔

    if(shuyecnt!=shuyeold)//时间间隔就不会出现突然跳变很大,数据非常稳定
    {
        shuyeold=shuyecnt;
        shuyesudu=60*1000*10/shuyetime;//比如2滴间隔1秒,1分钟就是60/1滴
        shuyetime=0;//清零时间
    }

    if(shuyetime>50000)//吊瓶打空了一般1分钟~2分钟会回血,1分钟12滴也很慢
    {
        shuyesudu=0;
        shuyecnt=0;
        shuyeold=0;
        shuyetime=0;
        shuyeti=300;//这样才可以重新计数
    }
}

void shuye_exit_int(void)
{

    if((hw==0)&&(shuyeti==0))//如果输液为低

    {
        shuyecnt++;//输液滴数+1
        shuyeti=3000;//输液速度一分钟60滴最多,所以,30ms延时可
    }
}
//按键启用输液
void key_start_shuye(void)
{
    shuyecnt=0;
    shuyeold=0;

    shuyeti=3000;

}