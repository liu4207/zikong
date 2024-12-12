#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//GPIO_Pin_15,�õ�PA0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 3000 - 1;		//ARR��ÿ�����ڵļ�����λ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;		//PSC,��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 15000;		//CCR����ʼռ�ձ�
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare1(float Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}

void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  // TIM1 ????
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // ?????
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  // ????
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // ????
    NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
    // ????? TIM1 ?????
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // ???????????,???? PWM ????????

        // ???????
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
void PA9_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // ʹ�� GPIOA ʱ��

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;               // ѡ�� PA9 ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // ��������ٶ�
    GPIO_Init(GPIOA, &GPIO_InitStructure);                    // ��ʼ�� PA9 ����

    GPIO_SetBits(GPIOA, GPIO_Pin_9);                          // ����ߵ�ƽ (VCC)
}
void B15_Init(void)
{
    // ʹ�� GPIOB ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ���� B15 ����Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;              // ѡ�� B15 ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         // �������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // ��������ٶ�
    GPIO_Init(GPIOB, &GPIO_InitStructure);                    // ��ʼ�� GPIOB �� B15 ����

    // ���� B15 ����Ϊ�͵�ƽ����ʾ�ӵ�
    GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}