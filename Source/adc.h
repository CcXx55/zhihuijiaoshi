vu8 flagadc = 0;

void InitADC(void)
{

	ADC_InitTypeDef ADC_InitStructure;

	/* Enable ADC1 and GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 12MHZ

	/* Configure PA.1 (ADC Channel) as analog input -------------------------*/

	ADC_DeInit(ADC1); // 在这里复位被我注释掉了、至于为什么，我待会会说

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // 这里对应上面所讲的配置，在这里就不给出注释了
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* Enable ADC1 */ // 知道我为啥要在上面提醒大家要用老外的方式来看了吧、因为这里的注释都是用英文的
	// 请不要以为我装逼，我这样做是有原因的、、原因我待会会说、你也会明白我最初的标题为何那样写

	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while (ADC_GetResetCalibrationStatus(ADC1))
		;

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADC1))
		;
}

// flaga=0,不转成百分比
u16 GetADCResult(u8 ch, vu8 flaga) // 得到１２位ＡＤＣ
{
	u16 DataValue; // 又是英文注释、、啊哈
	GPIO_InitTypeDef GPIO_InitStructure;
	if (flagadc == 0)
	{
		flagadc = 1;
		InitADC();
	}
	if (ch == 0)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	}
	if (ch == 1)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	}
	if (ch == 2)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	}
	if (ch == 3)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	}
	if (ch == 4)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	}
	if (ch == 5)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	}
	if (ch == 6)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	}
	if (ch == 7)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	}
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ADC1 regular channel14 configuration */
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	/* Test if the ADC1 EOC flag is set or not */

	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		;
	delayms(10);
	// FlagStatus Status;
	// Status = ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC);
	// while(!Status);---->这样做实现不了、请注意
	/*Returns the ADC1 Master data value of the last converted channel*/
	DataValue = ADC_GetConversionValue(ADC1);
	if (flaga)
		DataValue = DataValue / (4096.0 / 99.0);
	return DataValue;
}
