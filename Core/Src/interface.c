#include "interface.h"

/********************* 界面节点 ********************/
Interface_h *Start_Interface_H;
Interface_h *Main_Interface_H;

/********************* 选项节点 ********************/
// layer = 2
Interface_h *Timer_Interface_H;
Interface_h *Game_Interface_H;
Interface_h *Settings_Interface_H;
Interface_h *WaveVoltage_Interface_H;
Interface_h *DHT11_Interface_H;

// layer = 3
Interface_h *Alarm_Interface_H;
Interface_h *Stopwatch_Interface_H;
Interface_h *Time_Set_Interface_H;
Interface_h *Attitude_Calibration_Interface_H;
Interface_h *Showmode_H;
Interface_h *Bluetooth_H;
Interface_h *Dino_Interface_H;
Interface_h *WaveVoltage_H;
Interface_h *DHT_H;

// layer = 4
Interface_h *Alarm_H;
Interface_h *Stopwatch_H;
Interface_h *Time_Set_H;
Interface_h *Attitude_Calibration_H;
Interface_h *Dino_Play_H;

// layer = 5
Interface_h *Dino_H;

/********************* 功能节点 ******************/
// 实体函数
Interface_h *Minute;
Interface_h *Hour;
Interface_h *Year;
Interface_h *Month;
Interface_h *Day;
Interface_h *Week;
Interface_h *Complete;

// 交换节点
Interface_h *Select;
Interface_h *Current;

OLED_ColorMode Mode; // 屏幕模式
bool Shutdown;       // 关机
bool Funtion_ON;     // 功能开启/暂停

/********************* 打印时间 ********************/
timedata Clock = {"23", "00", "00", Stopwatch_Mode, false}; // 初始化时间数据
datedata Date = {"2024", "10", "29", "1"};                  // 初始化日期数据

void Print_Time(void)
{
  int8_t Increase = Interface_Y_Increase();

  OLED_PrintString(0, Increase, strncat(Date.year, "年", sizeof(Date.year) - strlen(Date.year) - 1), &font12x12, Mode);
  OLED_PrintString(4 * 6 + 12 + 5, Increase, strncat(Date.month, "月", sizeof(Date.month) - strlen(Date.month) - 1), &font12x12, Mode);
  OLED_PrintString(6 * 6 + 12 * 2 + 5, Increase, strncat(Date.day, "日", sizeof(Date.day) - strlen(Date.day) - 1), &font12x12, Mode);
  OLED_PrintString(128 - 12 * 3, Increase, Week_Judgment(Date.week), &font12x12, Mode);

  OLED_PrintString((OLED_COLUMN / 2 - 6 - 24 * 2), OLED_ROW - 24 - 12 - 14 + Increase, Clock.hour, &font24x24, Mode);
  if (State_Time)
  {
    OLED_PrintString((OLED_COLUMN - 24 * 2 - 22), (OLED_ROW - 24 - 12 - 14) + Increase, ":", &font24x24, Mode);
  }
  OLED_PrintString((OLED_COLUMN / 2 + 6), (OLED_ROW - 24 - 12 - 14) + Increase, Clock.minute, &font24x24, Mode);
  OLED_PrintString(OLED_COLUMN / 2 - 14 + 4, OLED_ROW - 21 + Increase, Clock.second, &font16x16, Mode);

  Time_Update(&Clock, Clock_Count);
}

/********************* 电量显示 ********************/
void Print_Power(void)
{
  OLED_DrawImage(OLED_COLUMN - 6 * 4 - dianchiImg.w, OLED_ROW - 10 + Interface_Y_Increase(), &dianchiImg, Mode);
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Interface_Y_Increase(), "100%", &font8x6, Mode);
}

/********************* 选框绘制 *********************/
void Print_Frame(void)
{
  uint8_t total = frame_y + Interface_Y_Increase();

  if (ReLU(total))
  {
    OLED_DrawLine(frame_x - 12, ReLU(total), frame_x, ReLU(total), Mode);           // 左上x
    OLED_DrawLine(frame_x + 32, ReLU(total), frame_x + 32 + 12, ReLU(total), Mode); // 右上x
  }
  OLED_DrawLine(frame_x - 12, ReLU(total + 32 + 12), frame_x, ReLU(total + 32 + 12), Mode);           // 左下x
  OLED_DrawLine(frame_x + 32, ReLU(total + 32 + 12), frame_x + 32 + 12, ReLU(total + 32 + 12), Mode); // 右下x
  OLED_DrawLine(frame_x - 12, ReLU(total), frame_x - 12, ReLU(total + 12), Mode);                     // 左上y
  OLED_DrawLine(frame_x + 32 + 12, ReLU(total), frame_x + 32 + 12, ReLU(total + 12), Mode);           // 右上y
  OLED_DrawLine(frame_x - 12, ReLU(total + 32), frame_x - 12, ReLU(total + 32 + 12), Mode);           // 左下y
  OLED_DrawLine(frame_x + 32 + 12, ReLU(total + 32), frame_x + 32 + 12, ReLU(total + 32 + 12), Mode); // 右下y
}

/********************* 箭头绘制 *********************/
void Print_Arrow(void)
{
  OLED_DrawImage(0, Interface_Y_Increase(), &left_arrowImg, Mode);
  OLED_DrawImage(OLED_COLUMN - rigth_arrowImg.w, Interface_Y_Increase(), &rigth_arrowImg, Mode);
}

/******************* 文字选框绘制 *******************/
void Print_Rectangle(Interface_h *Option)
{
  if (Current == Option && complete_interface)
  {
    if (Select->ID > 4)
    {
      OLED_DrawRectangle(64, String_Y_Increase() + left_arrowImg.h + 4 * (Select->ID - 5), String_X_Increase() + 4, 8 + 2, Mode);
    }
    else
    {
      OLED_DrawRectangle(0, String_Y_Increase() + left_arrowImg.h + 4 * (Select->ID - 1), String_X_Increase() + 4, 8 + 2, Mode);
    }
  }
}

/******************** 选项坐标增量 *******************/
uint8_t String_x, String_y, Interface_y, Pre_x = 0, Photo_x, Pre_ID, Pre_Name_length = 0;
int8_t Next_x;
bool state_photo, state_string, state_interface, complete_interface, switch_mode = true;
void (*Pre_Function)();

void Benchmarks_Init(void)
{
  while (1)
  {
    if (Interface_y < 64 / Interface_Y_Speed)
    {
      Interface_y++;
    }
    else
    {
      complete_interface = true;
    }

    if (Photo_x < 48 / Photo_X_Speed)
    {
      Photo_x++;
    }

    if (String_y < 8 / String_Y_Speed)
    {
      String_y++;
    }

    if (String_x < abs(Next_x))
    {
      String_x++;
    }

    vTaskSuspend(NULL);
  }
}

void Coordinate_Init(void)
{
  if (button.next)
  {
    Pre_x = Pre_Name_length / String_X_Speed;
    // Next_x = (6 * Select->Name_length) / String_X_Speed - Pre_x;
  }

  if (Pre_ID > Select->ID)
  {
    String_x = 0;
    Photo_x = 0;
    String_y = 0;
    state_string = false;
    state_photo = false;
  }

  if (Pre_ID < Select->ID)
  {
    String_x = 0;
    Photo_x = 0;
    String_y = 0;
    state_string = true;
    state_photo = true;
  }

  if (button.chose)
  {
    String_x = 0;
    Interface_y = 0;
    state_interface = true;
    Pre_x = 0;
  }

  if (button.quit)
  {
    String_x = 0;
    Interface_y = 0;
    state_interface = false;
  }

  Next_x = (6 * Select->Name_length) / String_X_Speed - Pre_x;
  Pre_ID = Select->ID;
}

uint8_t Photo_X_Increase(int8_t ID)
{
  uint8_t Increase;

  if (-(ID - 1) * 48 > (128 + 32) || -(ID - 1) * 48 < -48)
  {
    return 128;
  }

  Increase = state_photo ? (48 - Photo_X_Speed * Photo_x) - (ID - 1) * 48 : -(48 - Photo_X_Speed * Photo_x) - (ID - 1) * 48;

  return Increase;
}

int8_t String_Y_Increase(void)
{
  int8_t Increase;

  if (Select->ID > 4)
  {
    Increase = state_string ? (Select->ID - 6) * 8 + String_y * String_Y_Speed : (Select->ID - 4) * 8 - String_y * String_Y_Speed;
  }
  else
  {
    Increase = state_string ? (Select->ID - 2) * 8 + String_y * String_Y_Speed : Select->ID * 8 - String_y * String_Y_Speed;
  }

  return Increase;
}

uint8_t String_X_Increase(void)
{
  uint8_t Increase;

  if (String_x < abs(Next_x))
  {
    Increase = String_X_Speed * (Next_x < 0 ? Pre_x - String_x : Pre_x + String_x);
  }
  else
  {
    Pre_x = Pre_Name_length / String_X_Speed;
    Increase = Select->Name_length * 6;
  }
  Pre_Name_length = 6 * Select->Name_length;

  return Increase;
}

int8_t Interface_Y_Increase(void)
{
  int8_t Increase;

  if (!complete_interface)
  {
    Increase = state_interface ? -64 * switch_mode + Interface_y * Interface_Y_Speed : 64 * switch_mode - Interface_y * Interface_Y_Speed;
  }
  else
  {
    Increase = 0;
  }

  return Increase;
}

/********************* 一级菜单 *********************/
void Print_Main_Interface(void)
{
  int8_t Increase = Interface_Y_Increase();

  OLED_DrawImage(Photo_X_Increase(Select->ID - 1), Interface_Y_Increase() + Main_y, &TimerImg, Mode);
  OLED_DrawImage(Photo_X_Increase(Select->ID - 2), Interface_Y_Increase() + Main_y, &gameImg, Mode);
  OLED_DrawImage(Photo_X_Increase(Select->ID - 3), Interface_Y_Increase() + Main_y, &WaveVoltageImg, Mode);
  OLED_DrawImage(Photo_X_Increase(Select->ID - 4), Interface_Y_Increase() + Main_y, &dht11Img, Mode);
  OLED_DrawImage(Photo_X_Increase(Select->ID - 5), Interface_Y_Increase() + Main_y, &settingsImg, Mode);

  OLED_PrintString((OLED_COLUMN - 6 * Select->Name_length) / 2, OLED_ROW - 8 + Interface_Y_Increase(), Select->Name, &font8x6, Mode);
  OLED_PrintString(0, OLED_ROW - 8 + Increase, "Enter", &font8x6, Mode);
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
  Print_Frame();
  Print_Arrow();
}

/******************** 开始界面显示 ******************/
void Start_Interface(void)
{
  Print_Time();
  OLED_PrintString(0, OLED_ROW - 8 + Interface_Y_Increase(), "Menu", &font8x6, Mode);
  Print_Power();
  Print_Bluetooth();
}

/********************** 定/计时器 ********************/
void Print_Timer_Interface(void)
{
  int8_t Increase = Interface_Y_Increase();

  OLED_PrintString(2, left_arrowImg.h + Option_y(Alarm_Interface_H) + Increase, Alarm_Interface_H->Name, &font8x6, Mode);
  OLED_PrintString(2, left_arrowImg.h + Option_y(Stopwatch_Interface_H) + Increase, Stopwatch_Interface_H->Name, &font8x6, Mode);
  Print_Arrow();
  OLED_PrintString(2, OLED_ROW - 8 + Increase, "Enter", &font8x6, Mode);
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
  Print_Rectangle(Timer_Interface_H);
}

/************************ 闹钟 **********************/
void Print_Alarm_Interface(void)
{
  int8_t Increase = Interface_Y_Increase();
  bool state = Time_Set(&Alarm, NULL);

  Free_Option(Hour);
  Hour = Create_Option(Alarm_H, "Hour    ", NULL);
  Minute = Create_Option(Hour, "Minute  ", NULL);
  Complete = Create_Option(Minute, "Complete", NULL);

  if (!Select->ID)
  {
    Select = Hour;
    Pre_ID = 1;
  }

  if (state)
  {
    OLED_PrintString((OLED_COLUMN / 2 - 6 - 24 * 2), (OLED_ROW - 24 - 12 - 14), Alarm.hour, &font24x24, Mode);
    if (State_Time)
    {
      OLED_PrintString((OLED_COLUMN - 24 * 2 - 22), (OLED_ROW - 24 - 12 - 14), ":", &font24x24, Mode);
    }
    OLED_PrintString((OLED_COLUMN / 2 + 6), (OLED_ROW - 24 - 12 - 14), Alarm.minute, &font24x24, Mode);
    OLED_PrintString(OLED_COLUMN / 2 - 15 + 4, OLED_ROW - 21, Alarm.second, &font16x16, Mode);

    Timer_ON(Alarm_Mode);
  }
  else
  {
    OLED_PrintString(2, left_arrowImg.h + Increase + Option_y(Hour), strcat(Hour->Name, Alarm.hour), &font8x6, Mode);
    OLED_PrintString(2, left_arrowImg.h + Increase + Option_y(Minute), strcat(Minute->Name, Alarm.minute), &font8x6, Mode);
    OLED_PrintString(2, left_arrowImg.h + Increase + Option_y(Complete), Complete->Name, &font8x6, Mode);
    OLED_PrintString(0, OLED_ROW - 8 + Increase, "Increase", &font8x6, Mode);
  }
  Print_Arrow();
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
  if (!Complete_Alarm_Set)
  {
    Print_Rectangle(Alarm_Interface_H);
  }
}

/************************ 秒表 **********************/
void Print_Stopwatch_Interface(void)
{
  int8_t Increase = Interface_Y_Increase();

  OLED_PrintString((OLED_COLUMN / 2 - 6 - 24 * 2), (OLED_ROW - 24 - 12 - 14) + Increase, Stopwatch.hour, &font24x24, Mode);
  if (State_Time || Stopwatch.stop)
  {
    OLED_PrintString((OLED_COLUMN - 24 * 2 - 22), (OLED_ROW - 24 - 12 - 14) + Increase, ":", &font24x24, Mode);
  }
  OLED_PrintString((OLED_COLUMN / 2 + 6), (OLED_ROW - 24 - 12 - 14) + Increase, Stopwatch.minute, &font24x24, Mode);
  OLED_PrintString(OLED_COLUMN / 2 - 15 + 4, OLED_ROW - 21 + Increase, Stopwatch.second, &font16x16, Mode);
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
  if (Stopwatch.stop)
  {
    OLED_PrintString(0, OLED_ROW - 8 + Increase, "Start", &font8x6, Mode);
  }
  else
  {
    OLED_PrintString(0, OLED_ROW - 8 + Increase, "Suspend", &font8x6, Mode);
  }

  Timer_ON(Stopwatch_Mode);
}

/************************ 游戏 **********************/
void Print_Game_Interface(void)
{
  int8_t Increase = Interface_Y_Increase();

  OLED_PrintString(2, left_arrowImg.h + Option_y(Dino_Interface_H) + Increase, Dino_Interface_H->Name, &font8x6, Mode);
  Print_Arrow();
  OLED_PrintString(0, OLED_ROW - 8 + Increase, "Enter", &font8x6, Mode);
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
  Print_Rectangle(Game_Interface_H);
}

/*********************** 设置时间 *********************/
void Print_Time_Set_Interface(void)
{
  int8_t Increase = Interface_Y_Increase();

  Free_Option(Hour);
  Hour = Create_Option(Time_Set_H, "Hour    ", NULL);
  Minute = Create_Option(Hour, "Minute  ", NULL);
  Year = Create_Option(Minute, "Year    ", NULL);
  Month = Create_Option(Year, "Month   ", NULL);
  Day = Create_Option(Month, "Day     ", NULL);
  Week = Create_Option(Day, "Week    ", NULL);

  if (!Select->ID)
  {
    Select = Hour;
    Pre_ID = 1;
  }

  OLED_PrintString(2, left_arrowImg.h + Option_y(Hour) + Increase, strcat(Hour->Name, Clock.hour), &font8x6, Mode);
  OLED_PrintString(2, left_arrowImg.h + Option_y(Minute) + Increase, strcat(Minute->Name, Clock.minute), &font8x6, Mode);
  OLED_PrintString(2, left_arrowImg.h + Option_y(Year) + Increase, strcat(Year->Name, Date.year), &font8x6, Mode);
  OLED_PrintString(2, left_arrowImg.h + Option_y(Month) + Increase, strcat(Month->Name, Date.month), &font8x6, Mode);
  OLED_PrintString(66, left_arrowImg.h + Option_y(Day) + Increase, strcat(Day->Name, Date.day), &font8x6, Mode);
  OLED_PrintString(66, left_arrowImg.h + Option_y(Week) + Increase, strcat(Week->Name, Date.week), &font8x6, Mode);
  Print_Arrow();
  OLED_PrintString(2, OLED_ROW - 8 + Increase, "Increase", &font8x6, Mode);
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
  Print_Rectangle(Time_Set_Interface_H);

  Time_Set(&Clock, &Date);
}

/*********************** 姿态校准 *********************/
void Print_Attitude_Calibration_Interface(void)
{
  int8_t Increase = Interface_Y_Increase();
  static bool state;

  if (!state)
  {
    OLED_PrintString((128 - 14 * 6) / 2, (64 - 8) / 2 + Increase, "Calibrating...", &font8x6, Mode);
  }

  if (complete_interface)
  {
    while (!state)
    {
      state = true;
      Attitude_Calibration();
    }
    OLED_PrintString((128 - 8 * 6) / 2, (64 - 8) / 2 + Increase, "Complete", &font8x6, Mode);
  }
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
}

/************************ 开关状态 **********************/
void Switch_Transform(Interface_h *Option, bool *state)
{
  int8_t Increase = Interface_Y_Increase();

  if (Select->ID == Option->ID)
  {
    if (button.chose)
    {
      *state = !*state;
    }
  }

  if (*state)
  {
    OLED_DrawImage(128 - Switch_ONImg.w, left_arrowImg.h + Option_y(Option) + Increase, &Switch_ONImg, Mode);
  }
  else
  {
    OLED_DrawImage(128 - Switch_ONImg.w, left_arrowImg.h + Option_y(Option) + Increase, &Switch_OFFImg, Mode);
  }
}

/************************ 设置 **********************/
Switch Transform = {false, false};
void Print_Settings_Interface(void)
{
  int8_t Increase = Interface_Y_Increase();

  OLED_PrintString(2, left_arrowImg.h + Option_y(Time_Set_Interface_H) + Increase, Time_Set_Interface_H->Name, &font8x6, Mode);
  OLED_PrintString(2, left_arrowImg.h + Option_y(Attitude_Calibration_Interface_H) + Increase, Attitude_Calibration_Interface_H->Name, &font8x6, Mode);
  OLED_PrintString(2, left_arrowImg.h + Option_y(Showmode_H) + Increase, Showmode_H->Name, &font8x6, Mode);
  Switch_Transform(Showmode_H, &Transform.Showmode);
  OLED_PrintString(2, left_arrowImg.h + Option_y(Bluetooth_H) + Increase, Bluetooth_H->Name, &font8x6, Mode);
  Switch_Transform(Bluetooth_H, &Transform.Bluetooth);
  Print_Arrow();
  OLED_PrintString(2, OLED_ROW - 8 + Increase, "Enter", &font8x6, Mode);
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
  Print_Rectangle(Settings_Interface_H);
}

/********************* 蓝牙开启 ********************/
void Print_Bluetooth(void)
{
  if (Transform.Bluetooth)
  {
    OLED_DrawImage(OLED_COLUMN - BluetoothImg.w, OLED_ROW / 2 - BluetoothImg.h + Interface_Y_Increase(), &BluetoothImg, Mode);
  }
}

/****************** 设置功能开启/关闭 ****************/
TaskHandle_t xBlueTooth_Handle;
void Settings_Function(void)
{
  if (Transform.Bluetooth)
  {
    BlueTooth_ON();
    if (xBlueTooth_Handle == NULL)
    {
      xTaskCreate((TaskFunction_t)BlueTooth_ON, "BlueTooth", 256, NULL, osPriorityNormal, &xBlueTooth_Handle);
    }
    else
    {
      vTaskResume(xBlueTooth_Handle); // 可通过对提高动画任务的优先级确保动画的流畅性而不采用此方法
    }
  }
  else
  {
    if (xBlueTooth_Handle != NULL)
    {
      vTaskDelete(xBlueTooth_Handle);
      xBlueTooth_Handle = NULL;
    }
  }

  if (Transform.Showmode)
  {
    Mode = OLED_MODE_REVERSED;
  }
  else
  {
    Mode = OLED_MODE_NORMAL;
  }
}

/********************* 开始菜单 **********************/
void Interface_Init(void)
{
  Start_Interface_H = Create_HeadInterface(Start_Interface);

  Main_Interface_H = Create_ConnectInterface(Start_Interface_H, NULL, Print_Main_Interface, Photo_Mode);
  Timer_Interface_H = Create_Option(Main_Interface_H, "Timer", Print_Timer_Interface);
  Game_Interface_H = Create_Option(Timer_Interface_H, "Game", Print_Game_Interface);
  WaveVoltage_Interface_H = Create_Option(Game_Interface_H, "WV", Wave_Voltage);
  DHT11_Interface_H = Create_Option(WaveVoltage_Interface_H, "DHT11", DHT_Read);
  Settings_Interface_H = Create_Option(DHT11_Interface_H, "Settings", Print_Settings_Interface);

  // Timer_Interface_H
  Alarm_Interface_H = Create_ConnectInterface(Timer_Interface_H, "Alarm", Print_Alarm_Interface, String_Mode);
  Stopwatch_Interface_H = Create_Option(Alarm_Interface_H, "Stopwatch", Print_Stopwatch_Interface);

  Alarm_H = Create_TailInterface(Alarm_Interface_H);
  Stopwatch_H = Create_TailInterface(Stopwatch_Interface_H);

  // Game_Interface_H
  Dino_Interface_H = Create_ConnectInterface(Game_Interface_H, "Dino", introMessage, String_Mode);
  Dino_Play_H = Create_ConnectInterface(Dino_Interface_H, "Play", play, String_Mode);
  Dino_H = Create_TailInterface(Dino_Play_H);

  // WaveVoltage_Interface_H
  WaveVoltage_H = Create_TailInterface(WaveVoltage_Interface_H);

  // DHT11_Interface_H
  DHT_H = Create_TailInterface(DHT11_Interface_H);

  // Settings_Interface_H
  Time_Set_Interface_H = Create_ConnectInterface(Settings_Interface_H, "Timeset", Print_Time_Set_Interface, String_Mode);
  Attitude_Calibration_Interface_H = Create_Option(Time_Set_Interface_H, "Attitude", Print_Attitude_Calibration_Interface);
  Showmode_H = Create_Option(Attitude_Calibration_Interface_H, "Showmode", NULL);
  Bluetooth_H = Create_Option(Showmode_H, "Bluetooth", NULL);

  Time_Set_H = Create_TailInterface(Time_Set_Interface_H);
  Attitude_Calibration_H = Create_TailInterface(Attitude_Calibration_Interface_H);

  Current = Start_Interface_H;
  Select = Start_Interface_H;
  Pre_Function = Start_Interface_H->Function;
}

/********************* 进入菜单 *********************/
void Interface_Into(void)
{
  if (Select->Next_Interface != NULL && !Shutdown)
  {
    complete_interface = false;
    Pre_Function = Current->Function;

    if (!Current->ID)
    {
      Current = Select;
    }
    else
    {
      if (Current->layer == Select->layer)
      {
        Current = Select->Next_Interface;
      }
      else
      {
        Current = Select;
      }
    }

    if (!Current->ID)
    {
      Select = Current->Next_Option;
    }
    else
    {
      Select = Current->Next_Interface;
    }
  }
  else
  {
    if (Shutdown)
    {
      complete_interface = false;
      Shutdown = false;
    }

    if (Pre_Function == Main_Interface_H->Function)
    {
      Pre_Function = Start_Interface_H->Function;
    }
    OLED_DisPlay_On();
  }
}

/********************* 返回菜单 *********************/
void Interface_Return(void)
{
  if (Select->Pre_Interface != NULL)
  {
    complete_interface = false;
    Pre_Function = Current->Function;

    if (Current->layer != Select->layer)
    {
      Select = Select->Pre_Interface;
    }
    else
    {
      Select = Current->Pre_Interface;
    }

    Current = Current->Pre_Interface;
  }
  else
  {
    Shutdown = true;
  }
}

/********************* 下一选项 *********************/
void Option_Next(void)
{
  if (Select->Next_Option != NULL)
  {
    Select = Select->Next_Option;
  }
}

/********************* 上一选项 *********************/
void Option_Pre(void)
{
  if (Select->Pre_Option != NULL)
  {
    Select = Select->Pre_Option;
  }
}

/********************* 按键监测 *********************/
buttondata button = {0, 0, 0, 0};

void Button_Scan(void)
{
  while (1)
  {
    if (button.chose)
    {
      Interface_Into();
      Coordinate_Init();
      Buzzer_ON(short_beep);
      if (complete_interface)
      {
        Funtion_ON = !Funtion_ON;
      }
      button.chose = false;
    }
    if (button.quit)
    {
      Interface_Return();
      Coordinate_Init();
      Buzzer_ON(short_beep);
      Funtion_ON = false;
      button.quit = false;
    }

    if (button.pre)
    {
      Option_Pre();
      Coordinate_Init();
      Buzzer_ON(short_beep);
      button.pre = false;
    }

    if (button.next)
    {
      Option_Next();
      Coordinate_Init();
      Buzzer_ON(short_beep);
      button.next = false;
    }

    vTaskSuspend(NULL);
  }
}

/********************* 平滑过渡 *********************/
void Smooth_Transition(void)
{
  if (!complete_interface)
  {
    if (Pre_Function != Start_Interface_H->Function && Current != Dino_Play_H || Current == Main_Interface_H)
    {
      switch_mode = false;
      Pre_Function();
      switch_mode = true;
    }

    // vTaskPrioritySet(NULL, osPriorityNormal + 1);
    // vTaskPrioritySet(xBenchmarks_Handle, osPriorityNormal + 1);
    // vTaskPrioritySet(xButton_Handle, osPriorityNormal + 1);
  }
  /* 通过对提高动画任务的优先级确保动画的流畅性 */
  // else
  // {
  //   vTaskPrioritySet(NULL, osPriorityNormal);
  //   vTaskPrioritySet(xBenchmarks_Handle, osPriorityNormal);
  //   vTaskPrioritySet(xButton_Handle, osPriorityNormal);
  // }
}
/********************* 函数运行 *********************/
void Function_Run(void)
{
  while (1)
  {
    // Watch_Flip();
    if (!Shutdown)
    {
      OLED_NewFrame(Mode);
      Current->Function();
      Smooth_Transition();
      memset(&button, false, sizeof(button));
      Settings_Function();
      vTaskResume(xBenchmarks_Handle);
      OLED_ShowFrame();
    }
    else
    {
      if (HAL_GPIO_ReadPin(VOICE_GPIO_Port, VOICE_Pin))
      {
        button.chose = true;
      }
      OLED_DisPlay_Off();
    }

    vTaskResume(xButton_Handle);
  }
}
