

#define ON 1
#define OFF 3
//#define duoji2_port PA
//#define duoji2_bitt 11 // 若port=PA,bitt=11,代表舵机接PA11脚
#define duoji3_port PA
#define duoji3_bitt 11 // 若port=PA,bitt=11,代表舵机接PA11脚
#define duoji4_port PA
#define duoji4_bitt 11 // 若port=PA,bitt=11,代表舵机接PA11脚
// 3.3V那边可以不需要加电容,舵机不会影响

vu8 zb[4] = {255, 255, 255, 255};
void duoji(vu8 z, vu8 b) // 调用duoji(0,ON),b取值不能为0反复调用不会重复触发
{
	vu8 i;

	if (z == 0)
	{
		if (b == zb[0])
			return;
		zb[0] = b;
	}
	if (z == 1)
	{
		if (b == zb[1])
			return;
		zb[1] = b;
	}
	if (z == 2)
	{
		if (b == zb[2])
			return;
		zb[2] = b;
	}
	if (z == 3)
	{
		if (b == zb[3])
			return;
		zb[3] = b;
	}
	for (i = 0; i < 20; i++)
	{
		if (z == 0)
		{

			iow(duoji1_port, duoji1_bitt, out, 1);
			delayus(500 * b); // b取值不能为0
			iow(duoji1_port, duoji1_bitt, out, 0);
			delayus(20000 - 500 * b);
		}
		if (z == 1)
		{
			iow(duoji2_port, duoji2_bitt, out, 1);
			delayus(500 * b); // b取值不能为0
			iow(duoji2_port, duoji2_bitt, out, 0);
			delayus(20000 - 500 * b);
		}
		if (z == 2)
		{
			iow(duoji3_port, duoji3_bitt, out, 1);
			delayus(500 * b); // b取值不能为0
			iow(duoji3_port, duoji3_bitt, out, 0);
			delayus(20000 - 500 * b);
		}
		if (z == 3)
		{
			iow(duoji4_port, duoji4_bitt, out, 1);
			delayus(500 * b); // b取值不能为0
			iow(duoji4_port, duoji4_bitt, out, 0);
			delayus(20000 - 500 * b);
		}
	}
}