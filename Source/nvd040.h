// 好处在于是不用像JQ8400要生成音乐,还得拷贝

// 若port=PA,bitt=11,代表nvd040的dat脚接PA11脚

/*
 1.播放单曲
    nvd_one(1);


 2.组合播放温湿度(注意dat11是否超内存)
        nvdcnt=0;
        dat11[nvdcnt++]=10;//播放"温度"字眼
        qaa(TempNow);
        dat11[nvdcnt++]=13;//播放"度"字眼
        dat11[nvdcnt++]=11;//播放"湿度"字眼
        dat11[nvdcnt++]=14;//播放"百分比"字眼
        qaa(DampNow);
        nvd_duo(&dat11[0]);


 3.组合播放万年历(注意dat11是否超内存)
      nvdcnt = 0;
        dat11[nvdcnt++]=57;//播放"当前"字眼
        dat11[nvdcnt++]=58;//播放"时间"字眼
        qaa(year);
        dat11[nvdcnt++]=61;//播放"年"字眼
        qaa(month);
        dat11[nvdcnt++]=62;//播放"月"字眼
        qaa(date);
        dat11[nvdcnt++]=63;//播放"日"字眼

        qaa(hour);
        dat11[nvdcnt++]=12;//播放"点"字眼
        qaa(min);
        dat11[nvdcnt++]=59;//播放"分"字眼
        qaa(sec);
        dat11[nvdcnt++]=60;//播放"秒"字眼
        nvd_duo(&dat11[0]);
*/

/*
内容列表(发送10，才是播放温度)
11-温度12-湿度13-点14-度15-百分之16-十17-PM2.518-距离19-压力20-酒精21-浓度22-心率23-脉搏24-血氧25-声音26-震动
27-二氧化碳28-MQ229-光照30-土壤31-水位32-卡号33-MQ7
34-一氧化碳35-火焰36-水流37-金额38-请进39-欢迎光临40-电流41-电压42-伏43-厘米44-ppm45-干燥46-错误47-正确
48-3次错误49-请输入50-毫安51-充值    52-过近53-密码54-呼叫
55-我摔倒,帮帮我56-请刷卡57-余额58-当前59-时间60-分61-秒62-年63-月64-日65-有66-零点67-时间到了
68-异常69-过高70-过低71-不足
*/
vu8 nvdcnt = 0;
void delay5ms(void) // 89C52已经验证,只要延时准就能用
{
    delayms(5);
}
void delay400us(void) // 89C52已经验证,只要延时准就能用
{
    delay_us(420);
}

void delay1200us(void) // 89C52已经验证,只要延时准就能用
{
    delay_us(1260);
}
void sendbyte(vu8 dat)
{
    vu8 i;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x01)
        {
            iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 1);
            delay1200us();
            iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 0);
            delay400us();
        }
        else
        {
            iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 1);
            delay400us();
            iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 0);
            delay1200us();
        }
        dat >>= 1;
    }
}

// 播放10,不会播成1十0
void qaa(vu16 aa) // a为要播放的数值
{
    if (aa < 10)
        dat11[nvdcnt++] = aa % 10;
    else if (aa == 10)
        dat11[nvdcnt++] = 15; // 播放"十"
    else if (aa < 20)
    {
        dat11[nvdcnt++] = 15; // 播放"十"
        dat11[nvdcnt++] = aa % 10;
    }
    else if ((aa % 10) == 0)
    {
        dat11[nvdcnt++] = aa / 10;
        dat11[nvdcnt++] = 15; // 播放"十"
    }
    else

    {
        dat11[nvdcnt++] = aa / 10;
        dat11[nvdcnt++] = 15; // 播放"十"
        dat11[nvdcnt++] = aa % 10;
    }
}
vu16 tinv040 = 0;
void nvd_duo(vu8 *dat) // 组合播放
{
    vu8 i, sum = 0;
    if (tinv040)
        return;
    iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 0);
    delay5ms();
    //	sum=0xf1;
    sendbyte(0xf1);

    for (i = 0; i < nvdcnt; i++) // 循环指定次数
    {
        sendbyte(dat[i]);
        sum += dat[i];
    }

    sendbyte(0xf3);
    sendbyte(sum + 0xf1 + 0XF3);
    iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 1);
}
void nvd_one(vu8 dat) // dat就是播放的地址,一个地址
{
    vu8 i;
    if (tinv040)
        return;
    iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 0);
    delay5ms();
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x01)
        {
            iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 1);
            delay1200us();
            iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 0);
            delay400us();
        }
        else
        {
            iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 1);
            delay400us();
            iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 0);
            delay1200us();
        }
        dat >>= 1;
    }
    iow(MUSIC_DAT_port, MUSIC_DAT_bit, out, 1);
    delay1200us();
}
