#include "stm32f10x.h"                  // Device header  

void PWM_Init(void) {
    // Enable clocks for TIM1 and GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);

    // Configure PA8 as alternate function push-pull (TIM1_CH1)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // Use PA8 for TIM1_CH1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure TIM1 for PWM
    TIM_InternalClockConfig(TIM1);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 3000 - 1; // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    // Configure PWM output on TIM1 Channel 1
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500; // CCR, initial duty cycle
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    // Enable TIM1 counter
    TIM_Cmd(TIM1, ENABLE);

    // Enable TIM1 main output (MOE) for advanced timer
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void PWM_SetCompare1(float Compare) {
    // Ensure Compare is within bounds (0 - 3000)
    if (Compare < 0) Compare = 0;
    if (Compare > 3000) Compare = 3000;

    // Set the compare value for TIM1_CH1 (duty cycle control)
    TIM_SetCompare1(TIM1, (uint16_t)Compare); // Set CCR1 value to adjust duty cycle
}


void NVIC_Configuration(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  // Use TIM1 update interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM1_UP_IRQHandler(void) {
    // Handle TIM1 update interrupt
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        // Clear interrupt flag
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

        // Add your PWM-related logic here if needed
    }
}
