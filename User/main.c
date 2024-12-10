#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "AD.h"
#include "Serial.h"

#define PWM_FREQUENCY 10000    // PWM频率
#define MAX_PWM_VALUE 1000     // PWM最大值


#define PWM_PERIOD 3000  // PWM周期为1000个计数单位
#define KP 0.1          // 比例增益
#define KI 0.01         // 积分增益
#define KD 0.01
float voltage_ref = 12;  // 设定的输出电压 
float voltage_fb = 0.0;   // 反馈的实际输出电压
float duty_cycle = 0.0;   // 占空比
float error = 0.0;        // 误差
float integral = 0.0;     // 积分项
float control_signal = 0.0;  // 控制信号
// 定义全局变量
float last_error = 0.0;   // 上一次的误差

float derivative = 0.0;   // 微分项
int sample_index = 0;  // 样本计数器
float trueVoltage=0.0;     // 实际输出电压值
float VREF;
uint32_t time_counter = 0;  // 用于记录时间的计数器
uint8_t voltage_state = 0;  // 0 表示电压为8V，1 表示电压为10V
uint16_t ADValue;
float Voltage;
float i;

void control_buck(void)
{
	//读取ADC的值
	
	voltage_fb = (float)AD_GetValue()*3.3/4096;
    // 计算实际电压 (根据外部电路参数)
    trueVoltage = voltage_fb*6;
    // 计算误差
   error = voltage_ref/6 - voltage_fb;
	// 积分计算
   integral += error;
	// 微分计算    
   derivative = error - last_error;
	// PID控制输出
   control_signal = KP * error + KI * integral + KD * derivative;
	//control_signal *=1.09 ;
   if (control_signal > 0.8	)
   {
       control_signal = 0.8;
   }
   else if (control_signal < 0.2)
   {
       control_signal = 0.2;
   }
		// 更新占空比
   duty_cycle = control_signal * PWM_PERIOD;
	 i = (int)duty_cycle;
	 
	 //TIM1->CCR1 = (uint16_t)control_signal;
	 //TIM1->CCR1 = (uint16_t)duty_cycle;
	 PWM_SetCompare1(i);
	     Serial_Printf("%d,%.2f\r\n", sample_index, trueVoltage);  // 发送CSV格式的数据包
    sample_index++;
}

void update_voltage_reference(void)
{
	// 累计时间，每次延迟后调用此函数
	time_counter += 40;  // 假设主循环延时为10ms，每次累加10ms

	// 每10秒 (10000ms) 切换一次电压参考值
	if (time_counter >= 10000)
	{
		time_counter = 0;  // 重置计数器
		
		// 切换参考电压
	/*	if (voltage_state == 0)
		{
			voltage_ref = 10.0;  // 切换到10V
			voltage_state = 1;   // 更新状态
		}
		else
		{
			voltage_ref = 6.0;   // 切换到8V
			voltage_state = 0;   // 更新状态
		}*/
	}
}

int main(void)
{
	OLED_Init();
	PWM_Init();
	AD_Init();
	Delay_ms(10);
    NVIC_Configuration();  // ?? NVIC ??
//	Serial_Init();  // 初始化串口

	while (1)
	{
		update_voltage_reference();
		control_buck();
		
	}
}
