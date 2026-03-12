extern void LCD_Dis4bit(vu8 Line, vu8 Row, vu8 *sa, vu16 value1);

vu8 oldhuoer = 255;
vu8 inithuoer = 0;
// 取值0-255
// 只要每次上电速度值差不多在180-230之间就行,工作久,离心力让轴,轮子便宜,温度上升,速度会降
// 客户不信可以手感受是电机真的降速了,正好可以模拟真实测速
vu16 sudu = 0;
vu16 maichong = 0, cnt1s = 0;
/*******************************************************************************
 * Function Name  : EXTI15_10_IRQHandler
 * Description    : This function handles External lines 15 to 10 interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line15) != RESET) // 检查指定的EXTI1线路触发请求发生与否
	{
		EXTI_ClearITPendingBit(EXTI_Line15); // 清除EXTI0线路挂起位
		maichong++;
	}
}
void huoerinit(void)
{
	Exit_Init(GPIOB, GPIO_Pin_15, GPIO_Mode_IPU, EXTI_Trigger_Rising_Falling, 0, 0); // 必须上升下降沿,不然不能用
}
void huoerdeal(void) // 定时器1ms调用一次
{

	cnt1s++;
	if (cnt1s > 999)
	{
		cnt1s = 0;
		sudu = maichong / 30; // 1圈30个霍尔脉冲
		maichong = 0;
	}
}
