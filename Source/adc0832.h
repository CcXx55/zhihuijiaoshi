vu8 adc0832_vol1,adc0832_vol2;                        // 定位土壤水位
void adc0832deal(vu8 uart, vu8 flag) // flag=1转成百分比
{
    vu8 *a0832, *b0832;
    if (uart == 1)
    {
        a0832 = &Uart_RecOk1;
        b0832 = &Uart_Rx1[0];
    }
    else
    {
#ifdef stc_1T
        a0832 = &Uart_RecOk2;
        b0832 = &Uart_Rx2[0];
#endif
    }

    if (*a0832)
    {
        *a0832 = 0;

        if ((b0832[0] == 'S') && (b0832[6] == 'E')) // 在0x66与0x88直接的数据就是真实数据
        {
            adc0832_vol1 = ((b0832[1] - 48) * 10 + b0832[2] - 48); // 获取第一路电压,51代表5.1v
            adc0832_vol2 = ((b0832[4] - 48) * 10 + b0832[5] - 48); // 获取第2路电压,51代表5.1v

            if (flag)
            {
                // 换算成百分比
                adc0832_vol1 = adc0832_vol1 / (51.0 / 99.0);
               adc0832_vol2 = adc0832_vol2 / (51.0 / 99.0);
            }
        }
    }
		
}

