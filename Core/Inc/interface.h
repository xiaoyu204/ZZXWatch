#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "main.h"
#include "oled.h"
#include "font.h"
#include "function.h"
#include "Dino.h"

/*********************** 菜单坐标 **********************/
#define Main_y (64 - 32) / 2
#define Option_y(Option) (Option->ID < 5 ? (8 + 4) * (Option->ID - 1) + 2 : (8 + 4) * (Option->ID - 5) + 2)

/*********************** 选框坐标 **********************/
#define frame_x (128 - 32) / 2
#define frame_y (64 - 32) / 4

/*********************** 切换速度 **********************/
#define Photo_X_Speed 6     // 48
#define String_Y_Speed 1     // 8
#define String_X_Speed 2     // null
#define Interface_Y_Speed 8 // 64

/*********************** 时间参数 **********************/
extern bool State_Time;
extern uint32_t Clock_Count;

/*********************** 任务句柄 **********************/
extern TaskHandle_t xBenchmarks_Handle;
extern TaskHandle_t xButton_Handle;

/*********************** 绘制函数 **********************/
/* 绘制开始界面 */
void Print_Time(void);
void Print_Bluetooth(void);
void Print_Power(void);
/* 绘制UI */
void Print_Frame(void);
void Print_Arrow(void);
void Print_Rectangle(Interface_h *Option);
/* 开始界面 */
void Start_Interface(void);
/* 主菜单界面 */
void Print_Main_Interface(void);
/* 二级菜单界面 */
void Print_Timer_Interface(void);    // 定/计时器
void Print_Game_Interface(void);     // 游戏
void Print_Settings_Interface(void); // 设置
/* 三级菜单界面 */
void Print_Alarm_Interface(void);                // 闹钟
void Print_Stopwatch_Interface(void);            // 秒表
void Print_Time_Set_Interface(void);             // 时间设置
void Print_Attitude_Calibration_Interface(void); // 姿态校准

/*********************** 功能函数 **********************/
/* 界面初始化 */
void Interface_Init(void);
/* 坐标变换 */
void Coordinate_Init(void);          // 坐标初始化
void Benchmarks_Init(void);          // 基准点初始化
uint8_t Photo_X_Increase(int8_t ID); // 图片类坐标
int8_t String_Y_Increase(void);      // 文字类坐标
uint8_t String_X_Increase(void);
int8_t Interface_Y_Increase(void); // 界面坐标
/* 界面切换 */
void Interface_Into(void);   // 进入界面
void Interface_Return(void); // 返回界面
/* 选项切换 */
void Option_Next(void); // 下一个选项
void Option_Pre(void);  // 上一个选项
/* 开关状态 */
void Switch_Transform(Interface_h *Option, bool *state);
/* 设置功能开启/关闭 */
void Settings_Function(void);
/* 按钮扫描 */
void Button_Scan(void);
/* 平滑界面过渡 */
void Smooth_Transition(void);
/* 功能运行 */
void Function_Run(void);
/*******************************************************/
#endif // __interface_H_
