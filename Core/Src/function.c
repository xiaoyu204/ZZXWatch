#include "function.h"

/********************* 蓝牙模块 *********************/
uint8_t Receivedata[50];
void BlueTooth_ON(void)
{
    while (1)
    {
        extern TaskHandle_t xBlueTooth_Handle;

        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Receivedata, sizeof(Receivedata));
        __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
        vTaskSuspend(xBlueTooth_Handle); // 可通过对提高动画任务的优先级确保动画的流畅性而不采用此方法
    }
}

/******************** 开启蜂鸣器 ********************/
void Buzzer_ON(bool mode)
{
    if (mode) // 短鸣
    {
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
        osDelay(100);
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    }
    else // 长鸣
    {
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
    }
}

/********************* 更新时间 ********************/
void Time_Update(timedata *data, uint32_t count)
{
    static uint32_t carry;
    uint32_t sum_count = ((data->hour[0] - 48) * 10 + data->hour[1] - 48) * 3600 + (((data->minute[0] - 48) * 10 + data->minute[1] - 48) + (2 * data->mode - 1) * carry) * 60;

    carry = count / 60;

    if (data->mode) // 闹钟模式
    {
        if (!strstr(data->second, "00"))
        {
            sum_count += 60;
        }

        sum_count -= count;

        if (!sum_count)
        {
            data->stop = true;
            Time_Clear(&Alarm);
            Buzzer_ON(long_beep);
        }
    }
    else // 秒表模式
    {
        sum_count += count;
    }

    if (!data->stop)
    {
        data->second[1] = '0';
        data->second[1] += sum_count % 10;

        data->second[0] = '0';
        data->second[0] += sum_count % 60 / 10;

        data->minute[1] = '0';
        data->minute[1] += sum_count / 60 % 60 % 10;

        data->minute[0] = '0';
        data->minute[0] += sum_count / 60 % 60 / 10;

        data->hour[1] = '0';
        data->hour[1] += sum_count / 3600 % 10;

        data->hour[0] = '0';
        data->hour[0] += sum_count / 3600 / 10;
    }

    if (strstr(data->hour, "24"))
    {
        data->hour[0] = '0';
        data->hour[1] = '0';
    }

    /********************* 方案二 ********************/
    /****************** 仅限计时功能 *****************/
    // data -> second[1] = '0';
    // if (count % 10 != 0)
    // {
    //   data -> second[1] += count % 10;
    // }
    // else
    // {
    //   data -> second[0] = '0';
    //   data -> second[0] += count % 60 / 10;
    //   // if (strstr(data -> second, "60") != NULL)
    //   // {
    //   //   data -> second[0] = '0';
    //   //   data -> second[1] = '0';
    //   // }
    // }
    // if (count == 60)
    // {
    //   count = 0;
    //   data -> minute[1] ++;
    //   if (strchr(data -> minute, ':') != NULL)
    //   {
    //     data -> minute[1] = '0';
    //     data -> minute[0] ++;
    //   }
    //   if (strstr(data -> minute, "60"))
    //   {
    //     data -> minute[0] = '0';
    //     data -> minute[1] = '0';
    //     data -> hour[1] ++;
    //     if (strchr(data -> hour, ':') != NULL)
    //     {
    //       data -> hour[1] = '0';
    //       data -> hour[0] ++;
    //     }
    //     if (strstr(data -> hour, "24"))
    //     {
    //       data -> hour[0] = '0';
    //       data -> hour[1] = '0';
    //     }
    //   }
    // }
}

/********************* 清空时间 *********************/
void Time_Clear(timedata *data)
{
    data->second[1] = '0';
    data->second[0] = '0';
    data->minute[1] = '0';
    data->minute[0] = '0';
    data->hour[1] = '0';
    data->hour[0] = '0';
    data->stop = false;
}

/******************** 设置时间模块 ********************/
bool Complete_Alarm_Set = false;
bool Time_Set(timedata *data, datedata *day)
{
    if (Select->ID == Hour->ID)
    {
        if (button.chose)
        {
            data->hour[1]++;
            if (data->hour[1] > '9')
            {
                data->hour[1] = '0';
                data->hour[0]++;
            }

            if (data->hour[0] > '1' && data->hour[1] > '3')
            {
                data->hour[1] = '0';
                data->hour[0] = '0';
            }
        }
    }

    if (Select->ID == Minute->ID)
    {
        if (button.chose)
        {
            data->minute[1]++;
            if (data->minute[1] > '9')
            {
                data->minute[1] = '0';
                data->minute[0]++;
            }

            if (data->minute[0] > '5')
            {
                data->minute[1] = '0';
                data->minute[0] = '0';
            }
        }
    }

    if (Select->ID == Year->ID)
    {
        if (button.chose)
        {
            day->year[3]++;
            if (day->year[3] > '9')
            {
                day->year[3] = '0';
                day->year[2]++;
            }

            if (day->year[2] > '9')
            {
                day->year[2] = '0';
                day->year[3] = '0';
            }
        }
    }

    if (Select->ID == Month->ID)
    {
        if (button.chose)
        {
            day->month[1]++;
            if (day->month[1] > '9')
            {
                day->month[1] = '0';
                day->month[0]++;
            }

            if (day->month[0] > '0' && day->month[1] > '2')
            {
                day->month[0] = '0';
                day->month[1] = '1';
            }
        }
    }

    if (Select->ID == Day->ID)
    {
        if (button.chose)
        {
            day->day[1]++;
            if (day->day[1] > '9')
            {
                day->day[1] = '0';
                day->day[0]++;
            }

            if (day->day[0] > '2' && day->day[1] > '1')
            {
                day->day[0] = '0';
                day->day[1] = '1';
            }
        }
    }

    if (Select->ID == Week->ID)
    {
        if (button.chose)
        {
            day->week[0]++;
            if (day->week[0] > '7')
            {
                day->week[0] = '1';
            }
        }
    }

    if (Select->ID == Complete->ID)
    {
        if (button.chose)
        {
            Complete_Alarm_Set = true; // 设置完成
        }
    }

    if (button.quit)
    {
        data->second[0] = '0';
        data->second[1] = '0';
        if (Current == Timer_Interface_H)
        {
            Complete_Alarm_Set = false;
            HAL_TIM_Base_Stop_IT(&htim3);
            __HAL_TIM_SET_COUNTER(&htim3, 0);
            Timer_Count = 0;
            Time_Clear(&Alarm);
        }
    }

    return Complete_Alarm_Set;
}

/********************* 时钟模块 *********************/
timedata Stopwatch = {"00", "00", "00", Stopwatch_Mode, false};
timedata Alarm = {"00", "00", "00", Alarm_Mode, false};

void Timer_ON(bool mode)
{
    if (!mode) // 秒表
    {
        if (button.chose)
        {
            Stopwatch.stop = !Stopwatch.stop;
        }

        if (!Stopwatch.stop)
        {
            if (Current == Stopwatch_Interface_H)
            {
                HAL_TIM_Base_Start_IT(&htim3);
            }
            Time_Update(&Stopwatch, Timer_Count);
        }
        else
        {
            HAL_TIM_Base_Stop_IT(&htim3);
        }

        if (button.quit)
        {
            HAL_TIM_Base_Stop_IT(&htim3);
            __HAL_TIM_SET_COUNTER(&htim3, 0);
            Timer_Count = 0;
            Time_Clear(&Stopwatch);
        }
    }

    if (mode) // 闹钟
    {
        if (button.chose)
        {
            Alarm.stop = !Alarm.stop;
        }

        if (!Alarm.stop)
        {
            if (Current == Alarm_Interface_H)
            {
                HAL_TIM_Base_Start_IT(&htim3);
            }
        }

        Time_Update(&Alarm, Timer_Count);
    }
}

/******************** 判断星期几 *******************/
char *Week_Judgment(char *day)
{
    static char week[] = "星期一";

    switch (day[0])
    {
    case '1':
        week[6] = 0xE4; // 一   UTF-8编码
        week[7] = 0xB8;
        week[8] = 0x80;
        break;
    case '2':
        week[6] = 0xE4; // 二
        week[7] = 0xBA;
        week[8] = 0x8C;
        break;
    case '3':
        week[6] = 0xE4; // 三
        week[7] = 0xB8;
        week[8] = 0x89;
        break;
    case '4':
        week[6] = 0xE5; // 四
        week[7] = 0x9B;
        week[8] = 0x9B;
        break;
    case '5':
        week[6] = 0xE4; // 五
        week[7] = 0xBA;
        week[8] = 0x94;
        break;
    case '6':
        week[6] = 0xE5; // 六
        week[7] = 0x85;
        week[8] = 0xAD;
        break;
    case '7':
        week[6] = 0xE6; // 日
        week[7] = 0x97;
        week[8] = 0xA5;
        break;
    }

    return week;
}

/******************** 姿态校准 *******************/
float roll_f, yaw_f, pitch_f, roll, yaw, pitch;
void Attitude_Calibration(void)
{
    float roll_sum = 0, yaw_sum = 0, pitch_sum = 0;
    const uint16_t num_samples = 1000;

    for (uint16_t i = 0; i < num_samples; i++)
    {
        osDelay(10);
        MPU6050_DMP_Get_Date(&pitch, &roll, &yaw);
        roll_sum += roll;
        yaw_sum += yaw;
        pitch_sum += pitch;
        if (button.quit)
        {
            break;
        }
    }

    roll_f = roll_sum / num_samples;
    yaw_f = yaw_sum / num_samples;
    pitch_f = pitch_sum / num_samples;
}

/******************** 翻转手表 *******************/
void Watch_Flip(void)
{
    MPU6050_DMP_Get_Date(&pitch, &roll, &yaw);
    if (pitch_f - 1.f < pitch && pitch < pitch_f + 1.f && yaw_f - 1.f < yaw && yaw < yaw_f + 1.f)
    {
        Shutdown = false;
        OLED_DisPlay_On();
    }
}

/******************** 波形电压 *******************/
void Wave_Voltage(void)
{
    int8_t Increase = Interface_Y_Increase();
    static uint16_t Wave_Data[OLED_COLUMN];

    if (!complete_interface)
    {
        OLED_PrintString(0, OLED_ROW - 8 + Increase, "Start", &font8x6, Mode);
        OLED_DrawLine(0, 32 + Increase, 128, 32 + Increase, Mode);
        if (!Wave_Data[127])
        {
            Wave_Data[127] = 621;
        }
    }
    else
    {
        if (Funtion_ON)
        {
            uint16_t Data_Get;

            for (uint8_t i = 0; i < OLED_COLUMN; i++)
            {
                static uint16_t PreData;

                HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&Data_Get, 1);
                OLED_SetPixel(i, 32 - Data_Get / 128, Mode);
                OLED_DrawLine(i, PreData, i, 32 - Data_Get / 128, Mode);
                PreData = 32 - Data_Get / 128;
                Wave_Data[i] = Data_Get;
            }

            float Voltage = Data_Get / 4096.f * 3.3f - 0.5f;
            OLED_PrintFNum(2 + 8 * 6, 64 - 8 * 2 - 2, Voltage, &font8x6, Mode);
            OLED_PrintString(2, 64 - 8, "Suspend", &font8x6, Mode);
        }
        else
        {
            for (uint8_t i = 0; i < OLED_COLUMN; i++)
            {
                OLED_SetPixel(i, 32 - Wave_Data[i] / 128, Mode);
                if (i)
                {
                    OLED_DrawLine(i, 32 - Wave_Data[i - 1] / 128, i, 32 - Wave_Data[i] / 128, Mode);
                }
            }

            OLED_PrintFNum(2 + 8 * 6, 64 - 8 * 2 - 2, Wave_Data[127] / 4096.f * 3.3f - 0.5f, &font8x6, Mode);
            OLED_PrintString(2, 64 - 8, "Start", &font8x6, Mode);
        }

        OLED_PrintString(2, 64 - 8 * 2 - 2, "Voltage:", &font8x6, Mode);
        OLED_PrintString(2 + 12 * 6, 64 - 8 * 2 - 2, "V", &font8x6, Mode);
    }

    OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
}

/********************* us延迟 ********************/
void Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, reload, tcnt = 0;
    // if ((0x0001 & (SysTick->CTRL)) == 0) // 定时器未工作
    //     vPortSetupTimerInterrupt();      // 初始化定时器

    reload = SysTick->LOAD;                    // 获取重装载寄存器值
    ticks = nus * (SystemCoreClock / 1000000); // 计数时间值

    vTaskSuspendAll();   // 阻止OS调度，防止打断us延时
    told = SysTick->VAL; // 获取当前数值寄存器值（开始时数值）
    while (1)
    {
        tnow = SysTick->VAL; // 获取当前数值寄存器值
        if (tnow != told)    // 当前值不等于开始值说明已在计数
        {
            if (tnow < told)         // 当前值小于开始数值，说明未计到0
                tcnt += told - tnow; // 计数值=开始值-当前值

            else                              // 当前值大于开始数值，说明已计到0并重新计数
                tcnt += reload - tnow + told; // 计数值=重装载值-当前值+开始值  （
                                              // 已从开始值计到0）

            told = tnow; // 更新开始值
            if (tcnt >= ticks)
                break; // 时间超过/等于要延迟的时间,则退出.
        }
    }
    xTaskResumeAll(); // 恢复OS调度
}
// SystemCoreClock为系统时钟(system_stmf4xx.c中)，通常选择该时钟作为
// systick定时器时钟，根据具体情况更改

/******************** 激活函数 *******************/
uint8_t ReLU(int8_t input)
{
    return (input < 0) ? 0 : input;
}
