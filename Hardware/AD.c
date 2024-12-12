//AD��ʼ��
#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//����RCCʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//����ADCʱ�ӣ�ADC����APB2�ϵ�����
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);    // ���� ADC2 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//��PA1����GPIOA��ʱ��
//	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    // ���� GPIOB ʱ��
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);//ADC�ķ�Ƶ��������6��Ƶ12MHz
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������ģʽ��ADCר��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA1ģ�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
   // ���� PA10 Ϊģ�����루ADC2_Channel_10��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     // PA10 ����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
 // ���� ADC2 ��ͨ�� 10 (PA10)
    ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//��������ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�ⲿ����Դѡ������ط��ǃȲ��������
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	 // ��ʼ�� ADC2
    ADC_InitStructure.ADC_NbrOfChannel = 1;  // ֻ����һ��ͨ��
    ADC_Init(ADC2, &ADC_InitStructure);
		
	//���ö�ʱ�ж�
	ADC_Cmd(ADC1, ENABLE);
	   ADC_Cmd(ADC2, ENABLE);


	
	//У׼
	ADC_ResetCalibration(ADC1);//��λУ׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	// У׼ ADC2
    ADC_ResetCalibration(ADC2);  // ��λУ׼
    while (ADC_GetResetCalibrationStatus(ADC2) == SET);  // �ȴ���λ���
    ADC_StartCalibration(ADC2);  // ����У׼
    while (ADC_GetCalibrationStatus(ADC2) == SET);  // �ȴ�У׼���
		
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
	 // ���� ADC2 ת��
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);
    while (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);  // �ȴ�ת�����
    return ADC_GetConversionValue(ADC2);  // ��ȡ ADC2 ��ת��ֵ
}
