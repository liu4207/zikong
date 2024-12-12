#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "AD.h"
#include "Serial.h"


//#define PWM_FREQUENCY 10000    // PWMƵ��
//#define MAX_PWM_VALUE 1000     // PWM���ֵ
// ����⴫������ PWM ����
#define MAX_LIGHT_SENSOR_VALUE 3.3  // ����⴫����������Ϊ 3.3V
#define MIN_LIGHT_SENSOR_VALUE 0.0  // ����⴫���������СΪ 0V
float light_value = 0.0;  // �ӹ⴫������ȡ�Ĺ�ǿֵ

#define PWM_PERIOD 3000  // PWM����Ϊ1000��������λ
#define KP 0.1          // ��������
#define KI 0.01         // ��������
#define KD 0.01
float voltage_ref = 10;  // �趨�������ѹ 
float voltage_fb = 0.0;   // ������ʵ�������ѹ
float duty_cycle = 0.0;   // ռ�ձ�
float error = 0.0;        // ���
float integral = 0.0;     // ������
float control_signal = 0.0;  // �����ź�
// ����ȫ�ֱ���
float last_error = 0.0;   // ��һ�ε����

float derivative = 0.0;   // ΢����
int sample_index = 0;  // ����������
float trueVoltage=0.0;     // ʵ�������ѹֵ
float VREF;
uint32_t time_counter = 0;  // ���ڼ�¼ʱ��ļ�����
uint8_t voltage_state = 0;  // 0 ��ʾ��ѹΪ8V��1 ��ʾ��ѹΪ10V
uint16_t ADValue;
float Voltage;
float i;

void control_buck(void)
{
	//��ȡADC��ֵ
	
	voltage_fb = (float)AD_GetValue1()*3.3/4096;
    // ����ʵ�ʵ�ѹ (�����ⲿ��·����)
    trueVoltage = voltage_fb*1050*20000/2500/1000*0.5;
    // �������
   error = voltage_ref*2500*1000/1050/20000*2- voltage_fb;

	// ���ּ���
   integral += error;
	// ΢�ּ���    
   derivative = error - last_error;
	//last_error=error;
	// PID�������
   control_signal = KP * error + KI * integral + KD * derivative;
   if (control_signal > 0.8)
   {
       control_signal = 0.8;
   }
   else if (control_signal < 0.2)
   {
       control_signal = 0.2;
   }
		// ����ռ�ձ�
   duty_cycle = control_signal * PWM_PERIOD;
	 i = (int)duty_cycle;
	 
	 //TIM2->CCR1 = (uint16_t)control_signal;
	 //TIM2->CCR1 = (uint16_t)duty_cycle;
	 PWM_SetCompare1(i);
	     Serial_Printf("%d,%.2f\r\n", sample_index, trueVoltage);  // ����CSV��ʽ�����ݰ�
    sample_index++;
}

void update_voltage_reference(void)
{
	// �ۼ�ʱ�䣬ÿ���ӳٺ���ô˺���
	time_counter += 40;  // ������ѭ����ʱΪ10ms��ÿ���ۼ�10ms

	// ÿ10�� (10000ms) �л�һ�ε�ѹ�ο�ֵ
	if (time_counter >= 10000)
	{
		time_counter = 0;  // ���ü�����
		
//		// �л��ο���ѹ
//		if (voltage_state == 0)
//		{
//			voltage_ref = 10.0;  // �л���10V
//			voltage_state = 1;   // ����״̬
//		}
//		else
//		{
//			voltage_ref = 6.0;   // �л���8V
//			voltage_state = 0;   // ����״̬
//		}
	}
}
// ���Ƶ����ȵĺ���
void control_light(void)
{
    // ��ȡ�⴫������ģ��ֵ������ͨ�� ADC1 �� ADC2 ��ȡ��
    light_value = (float)AD_GetValue2() * 3.3 / 4096;  
    
    // ���⴫������ֵ����ӳ�䵽 PWM ռ�ձ�
    // ����⴫����Խ�����ֵԽС������Ŀ���ǣ���Խ������Խ��
    duty_cycle = (MAX_LIGHT_SENSOR_VALUE - light_value) / MAX_LIGHT_SENSOR_VALUE * PWM_PERIOD;
    
    // ��ռ�ձ������ں���Χ
    if (duty_cycle > PWM_PERIOD)
    {
        duty_cycle = PWM_PERIOD;
    }
    else if (duty_cycle < 0)
    {
        duty_cycle = 0;
    }
    
    // �������ź�ӳ�䵽 PWM ռ�ձ�
    PWM_SetCompare1((int)duty_cycle);
    
    // �����ǰ�Ĺ�ֵ�͵Ƶ�����
    Serial_Printf("%d, %.2f, %.2f\r\n", sample_index, light_value, duty_cycle);  // ���� CSV ��ʽ�����ݰ�
    sample_index++;
}


int main(void)
{
	 B15_Init();
	PA9_Init();
	OLED_Init();
	PWM_Init();
	AD_Init();
	Delay_ms(10);
    NVIC_Configuration();  // ?? NVIC ??
	Serial_Init();  // ��ʼ������

	while (1)
	{
//		update_voltage_reference();
		//control_buck();
		control_light();
	}
}
