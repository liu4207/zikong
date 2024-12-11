#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);
void PWM_SetCompare1(float Compare);
void NVIC_Configuration(void);
void TIM2_IRQHandler(void);
#endif
