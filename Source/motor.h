#include "stm32f10x_lib.h"

vu16 time_motor = 0;                                            // 定义电机转动倒计时
vu8 CCW1[8] = {0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09}; //????????
vu8 CCW2[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08}; //????????
vu8 *CCW = CCW1;
vu8 is = 0;

void motor_ccw(void)
{

    if (CCW[is] & 0X08)
        iow(cheng_port, cheng_bit, out, 1);

    else
        iow(cheng_port, cheng_bit, out, 0);

    if (CCW[is] & 0X04)
        iow(huang_port, huang_bit, out, 1);

    else
        iow(huang_port, huang_bit, out, 0);

    if (CCW[is] & 0X02)
        iow(fen_port, fen_bit, out, 1);
    else
        iow(fen_port, fen_bit, out, 0);

    if (CCW[is] & 0X01)
        iow(lan_port, lan_bit, out, 1);
    else
        iow(lan_port, lan_bit, out, 0);
    is++;
    if (is > 7)
        is = 0;
}

void motor_stop(void)
{
    iow(cheng_port, cheng_bit, out, 0);
    iow(huang_port, huang_bit, out, 0);
    iow(fen_port, fen_bit, out, 0);
    iow(lan_port, lan_bit, out, 0);
}
void motor_int(void) // 定时器1ms调用
{
    if (time_motor) // 电机转动倒计时不为0
    {
        time_motor--; // 倒计时减1
        motor_ccw();  // 电机转
    }
}
void open(void) // 设置电机打开
{
    if (CCW == CCW2) // 如果电机处于关闭状态
    {
        CCW = CCW1;        // 设置电机转动方向--打开
        time_motor = 3000; // 设置电机转动时间,单位ms
    }
}
void close(void) // 设置电机关闭
{
    if (CCW == CCW1) // 如果电机处于打开状态
    {
        CCW = CCW2;        // 设置电机转动方向---关闭
        time_motor = 3000; // 设置电机转动时间,单位ms
    }
}
