#define pin_clk PAin(5)
#define pin_dat PAin(4)
vu8 swlinshi=0,swval=0,swcnt=0,swhig=0,swold=1;

//定时1ms调用,得到数据,就是swval,
//0代表没有水,1为5mm,2为10mm...最多为9
void shuiweicheck(void)
{
    if(pin_clk)
    {



        swhig++;//连续高电平,则清零参数
        if(swhig>6)
        {
            swlinshi=0;
            swold=1;
            swcnt=0;
            swhig=0;

        }

    }
    if(pin_clk!=swold)
    {
        swold=pin_clk;
        if(swold==0)
        {

            swlinshi>>=1;
            if(pin_dat)
                swlinshi|=0x80;
            swcnt++;
            if(swcnt>=8)
            {
                swcnt=0;
                swval=swlinshi;
                swlinshi=0;
            }
            swhig=0;
        }
    }
}

