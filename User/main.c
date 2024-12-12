#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "AD.h"
#include "Serial.h"


//#define PWM_FREQUENCY 10000    // PWM频率
//#define MAX_PWM_VALUE 1000     // PWM最大值
// 定义光传感器和 PWM 参数
#define MAX_LIGHT_SENSOR_VALUE 3.3  // 假设光传感器输出最大为 3.3V
#define MIN_LIGHT_SENSOR_VALUE 0.0  // 假设光传感器输出最小为 0V
float light_value = 0.0;  // 从光传感器读取的光强值

#define PWM_PERIOD 3000  // PWM周期为1000个计数单位
#define KP 0.1          // 比例增益
#define KI 0.01         // 积分增益
#define KD 0.01
float voltage_ref = 10;  // 设定的输出电压 
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
	
	voltage_fb = (float)AD_GetValue1()*3.3/4096;
    // 计算实际电压 (根据外部电路参数)
    trueVoltage = voltage_fb*1050*20000/2500/1000*0.5;
    // 计算误差
   error = voltage_ref*2500*1000/1050/20000*2- voltage_fb;

	// 积分计算
   integral += error;
	// 微分计算    
   derivative = error - last_error;
	//last_error=error;
	// PID控制输出
   control_signal = KP * error + KI * integral + KD * derivative;
   if (control_signal > 0.8)
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
	 
	 //TIM2->CCR1 = (uint16_t)control_signal;
	 //TIM2->CCR1 = (uint16_t)duty_cycle;
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
		
//		// 切换参考电压
//		if (voltage_state == 0)
//		{
//			voltage_ref = 10.0;  // 切换到10V
//			voltage_state = 1;   // 更新状态
//		}
//		else
//		{
//			voltage_ref = 6.0;   // 切换到8V
//			voltage_state = 0;   // 更新状态
//		}
	}
}
// 控制灯亮度的函数
void control_light(void)
{
    // 读取光传感器的模拟值（假设通过 ADC1 和 ADC2 获取）
    light_value = (float)AD_GetValue2() * 3.3 / 4096;  
    
    // 将光传感器的值反向映射到 PWM 占空比
    // 假设光传感器越暗输出值越小，控制目标是：光越暗，灯越亮
    duty_cycle = (MAX_LIGHT_SENSOR_VALUE - light_value) / MAX_LIGHT_SENSOR_VALUE * PWM_PERIOD;
    
    // 将占空比限制在合理范围
    if (duty_cycle > PWM_PERIOD)
    {
        duty_cycle = PWM_PERIOD;
    }
    else if (duty_cycle < 0)
    {
        duty_cycle = 0;
    }
    
    // 将控制信号映射到 PWM 占空比
    PWM_SetCompare1((int)duty_cycle);
    
    // 输出当前的光值和灯的亮度
    Serial_Printf("%d, %.2f, %.2f\r\n", sample_index, light_value, duty_cycle);  // 发送 CSV 格式的数据包
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
	Serial_Init();  // 初始化串口

	while (1)
	{
//		update_voltage_reference();
		//control_buck();
		control_light();
	}
}
