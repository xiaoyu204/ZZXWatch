#ifndef __FUNCTION_H_
#define __FUNCTION_H_

#include "main.h"
#include "usart.h"
#include "cmsis_os.h"
#include "tim.h"
#include "my_list.h"
#include "adc.h"
#include "oled.h"
#include "dht11.h"
#include "MPU6050_dmp.h"
#include "FreeRTOS.h"

/*********************** 时钟模式 **********************/
#define Alarm_Mode true
#define Stopwatch_Mode false

/********************** 蜂鸣器模式 *********************/
#define short_beep 1
#define long_beep 0

/******************** 功能开启/关闭 ********************/
typedef struct Switch
{
  bool Bluetooth;
  bool Showmode;
} Switch;

typedef struct timedata
{ // 时间类
  char hour[3];
  char minute[3];
  char second[3];
  bool mode;
  bool stop;
} timedata;

typedef struct datedata
{ // 日期类
  char year[8];
  char month[6];
  char day[6];
  char week[2];
} datedata;

typedef struct buttondata
{ // 按键类
  bool pre;
  bool next;
  bool chose;
  bool quit;
} buttondata;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern buttondata button;
extern uint32_t Timer_Count;
extern bool Complete_Alarm_Set;
extern timedata Alarm;
extern timedata Stopwatch;
extern bool complete_interface;
extern bool Shutdown;
extern bool Funtion_ON;

/************************* 界面 ************************/
extern Interface_h *DHT11_Interface_H;
extern Interface_h *WaveVoltage_Interface_H;
extern Interface_h *Dino_Play_H;
extern Interface_h *Timer_Interface_H;
extern Interface_h *Alarm_Interface_H;
extern Interface_h *Stopwatch_Interface_H;
extern Interface_h *Minute;
extern Interface_h *Hour;
extern Interface_h *Complete;
extern Interface_h *Year;
extern Interface_h *Month;
extern Interface_h *Day;
extern Interface_h *Week;
extern Interface_h *Select;
extern Interface_h *Current;

extern int8_t Interface_Y_Increase(void);
extern void Benchmarks_Init(void);

/* 如果x大于0，输出x；否则，输出0 */
uint8_t ReLU(int8_t input);
/* 开启蓝牙 */
void BlueTooth_ON(void);
/* 开启蜂鸣器 */
void Buzzer_ON(bool mode);
/* 开启定时器 */
void Timer_ON(bool mode);
/* 更新时间 */
void Time_Update(timedata *data, uint32_t count);
/* 清空时间类数组 */
void Time_Clear(timedata *data);
/* 设置时间 */
bool Time_Set(timedata *data, datedata *day);
/* 判断星期几 */
char *Week_Judgment(char *day);
/* 翻转手表 */
void Watch_Flip(void);
/* 姿态校准 */
void Attitude_Calibration(void);
/* 绘制波形和测量电压 */
void Wave_Voltage(void);
/* us延迟 */
void Delay_us(uint32_t nus);

#endif // __FUNCTION_H__
