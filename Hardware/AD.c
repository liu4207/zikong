//AD初始化
#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//开启RCC时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//开启ADC时钟，ADC都是APB2上的外设
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);    // 开启 ADC2 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//用PA1开启GPIOA的时钟
//	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    // 开启 GPIOB 时钟
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);//ADC的分频，这里是6分频12MHz
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入模式，ADC专属模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA1模拟输入的引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
   // 配置 PA10 为模拟输入（ADC2_Channel_10）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     // PA10 引脚
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
 // 配置 ADC2 的通道 10 (PA10)
    ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立工作模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发源选择，这个地方是內部软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	 // 初始化 ADC2
    ADC_InitStructure.ADC_NbrOfChannel = 1;  // 只配置一个通道
    ADC_Init(ADC2, &ADC_InitStructure);
		
	//配置定时中断
	ADC_Cmd(ADC1, ENABLE);
	   ADC_Cmd(ADC2, ENABLE);


	
	//校准
	ADC_ResetCalibration(ADC1);//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	// 校准 ADC2
    ADC_ResetCalibration(ADC2);  // 复位校准
    while (ADC_GetResetCalibrationStatus(ADC2) == SET);  // 等待复位完成
    ADC_StartCalibration(ADC2);  // 启动校准
    while (ADC_GetCalibrationStatus(ADC2) == SET);  // 等待校准完成
		
}

uint16_t AD_GetValue1(void)
{
	//ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}
uint16_t AD_GetValue2(void)
{
	 // 启动 ADC2 转换
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);
    while (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);  // 等待转换完成
    return ADC_GetConversionValue(ADC2);  // 获取 ADC2 的转换值
}
