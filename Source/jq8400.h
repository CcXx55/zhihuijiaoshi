
vu8 jq8400cnt = 0; // 全局
vu8 ti8400 = 0;
// 播放歌曲几
void jq8400_Signle(vu8 uart, vu8 a)
{

	if (ti8400)
		return;
	ti8400 = 50;
	sendchar(uart, 0xaa);
	sendchar(uart, 0x1b);
	sendchar(uart, 0x2);
	sendchar(uart, 0x30 + (a / 10));
	sendchar(uart, 0x30 + (a % 10));
	sendchar(uart, 0xaa + 0x1b + 2 + 0x30 + (a / 10) + 0x30 + (a % 10));
}

/*
			播放年月日时分秒(微云有音频)
				jq8400cnt=0;
				dat11[jq8400cnt++]=26;//当前时间
				dat11[jq8400cnt++]=2;
				dat11[jq8400cnt++]=0;
				dat11[jq8400cnt++]=2;
				dat11[jq8400cnt++]=2;
				dat11[jq8400cnt++]=20;//年

				qaa(month);
				dat11[jq8400cnt++]=21;//月
				qaa(date);
				dat11[jq8400cnt++]=22;//日

				qaa(hour);
				dat11[jq8400cnt++]=23;//点
				qaa(min);
				dat11[jq8400cnt++]=24;//分
				qaa(sec);
				dat11[jq8400cnt++]=25;//秒
				jq8400_play(1,&dat11[0],jq8400cnt);


			播放温湿度(微云有音频)
				jq8400cnt=0;
				dat11[jq8400cnt++]=16;//当前温度
				qaa(TempNow);
				dat11[jq8400cnt++]=17;//度

				dat11[jq8400cnt++]=18;//当前湿度
				dat11[jq8400cnt++]=19;//百分之
				qaa(DampNow);
				jq8400_play(1,&dat11[0],jq8400cnt);

			如果数值3位
				vu16 distance=112;
					jq8400cnt=0;
					dat11[jq8400cnt++]=16;
					if(distance==100)
					{
							dat11[jq8400cnt++]=1;
							dat11[jq8400cnt++]=12;
					}
					else if(distance>100)
					{
							dat11[jq8400cnt++]=distance/100;
							dat11[jq8400cnt++]=12;
							aa=distance%100;
							if(aa<10)
							{
								dat11[jq8400cnt++]=0;
								dat11[jq8400cnt++]=aa%10;
							}

							else if((aa%10)==0)
							{
								dat11[jq8400cnt++]=aa/10;


							}
							else

							{
								dat11[jq8400cnt++]=aa/10;
								dat11[jq8400cnt++]=10;	//播放"十"
								dat11[jq8400cnt++]=aa%10;
							}
					}
					else
						qaa(distance%100);

					jq8400_play(1,&dat11[0],jq8400cnt);
*/

void jq8400_play(vu8 uart, char *ag, char recvcnt)
{

	unsigned char i = 0, sum2;

	if (ti8400)
		return;
	ti8400 = 50;

	sendchar(uart, 0xaa);
	sendchar(uart, 0x1b);

	sendchar(uart, recvcnt * 2);

	sum2 = 0xaa + 0x1b + recvcnt * 2;
	for (i = 0; i < recvcnt; i++)
	{
		sendchar(uart, ag[i] / 10 + 48);
		sendchar(uart, ag[i] % 10 + 48);
		sum2 += (ag[i] / 10 + 48);
		sum2 += (ag[i] % 10 + 48);
	}
	sendchar(uart, sum2);
}
// 停止播放音乐
void jq8400_stop(vu8 uart)
{
	sendchar(uart, 0xaa);
	sendchar(uart, 0x4);
	sendchar(uart, 0x0);
	sendchar(uart, 0xae);
}

// 音量,取值0-3uart,0静音
void jq8400_voice(vu8 uart, vu8 vol)
{

	sendchar(uart, 0xaa);
	sendchar(uart, 0x13);
	sendchar(uart, 0x1);
	sendchar(uart, vol * 10);
	sendchar(uart, 0xaa + 0x13 + 1 + vol * 10);
}

// 播放10,不会播成1十0
void qaa(vu16 aa) // a为要播放的数值
{
	if (aa < 10)
		dat11[jq8400cnt++] = aa % 10;
	else if (aa == 10)
		dat11[jq8400cnt++] = 10; // 播放"十"
	else if (aa < 20)
	{
		dat11[jq8400cnt++] = 10; // 播放"十"
		dat11[jq8400cnt++] = aa % 10;
	}
	else if ((aa % 10) == 0)
	{
		dat11[jq8400cnt++] = aa / 10;
		dat11[jq8400cnt++] = 10; // 播放"十"
	}
	else

	{
		dat11[jq8400cnt++] = aa / 10;
		dat11[jq8400cnt++] = 10; // 播放"十"
		dat11[jq8400cnt++] = aa % 10;
	}
}
