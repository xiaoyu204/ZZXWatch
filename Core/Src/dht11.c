#include "dht11.h"

char Data[5];
// 8bit 湿度整数数据[0] + 8bit 湿度小数数据[1] + 8bit 温度整数数据[2] + 8bit 温度小数数据[3] + 8bit 校验位[4]

void DHT_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.Pin = DHT11_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DHT11_IO, &GPIO_InitStructure);
}

void DHT_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.Pin = DHT11_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DHT11_IO, &GPIO_InitStructure);
}

/**
 * @brief       DHT11起始工作
 * @param       无
 * @retval      无
 */
void DHT11_Start()
{
  DHT_GPIO_OUTPUT();
  HAL_GPIO_WritePin(DHT11_IO, DHT11_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(DHT11_IO, DHT11_PIN, GPIO_PIN_RESET);
  vTaskDelay(20); // 拉低总线至少 18ms
  HAL_GPIO_WritePin(DHT11_IO, DHT11_PIN, GPIO_PIN_SET);

  DHT_GPIO_INPUT();

  while (HAL_GPIO_ReadPin(DHT11_IO, DHT11_PIN))
    ; // 上一步将总线设为高电平，等待DHT11响应低电平
  while (!HAL_GPIO_ReadPin(DHT11_IO, DHT11_PIN))
    ; // 上一步DHT11响应低电平，等待DHT11拉高总线
  while (HAL_GPIO_ReadPin(DHT11_IO, DHT11_PIN))
    ; // 上一步DHT11拉高了总线，等待DHT11拉低总线，开始传送数据
}

/**
 * @brief       DHT11读一字节
 * @param       无
 * @retval      ReadData-读取的数据结果
 */
uint8_t DHT_Read_Byte(void) // 从DHT11读取一位（8字节）信号
{
  uint8_t i;
  uint8_t ReadData = 0; // ReadData用于存放8bit数据，即8个单次读取的1bit数据的组合
  uint8_t temp;         // 临时存放信号电平（0或1）

  for (i = 0; i < 8; i++)
  {
    while (!HAL_GPIO_ReadPin(DHT11_IO, DHT11_PIN))
      ;
    Delay_us(50);
    if (HAL_GPIO_ReadPin(DHT11_IO, DHT11_PIN) == 1)
    {
      temp = 1;
      while (HAL_GPIO_ReadPin(DHT11_IO, DHT11_PIN))
        ;
    }
    else
    {
      temp = 0;
    }
    ReadData = ReadData << 1;
    ReadData |= temp;
  }
  return ReadData;
}

void DHT_Read()
{
  int8_t Increase = Interface_Y_Increase();
  static float Tempertature, Humidity;
  static uint32_t time_out;
  bool dalay_c = false;

  if (uwTick - time_out > 2000)
  {
    time_out = uwTick;
    dalay_c = true;
  }

  if (complete_interface && dalay_c)
  {
    DHT11_Start();

    for (uint8_t i = 0; i < 5; i++)
    {
      Data[i] = DHT_Read_Byte();
    }
    HAL_InitTick(uwTickPrio);

    if ((Data[0] + Data[1] + Data[2] + Data[3]) == Data[4])
    {
      Humidity = (Data[0] + Data[1] * 0.1f);
      Tempertature = (Data[2] + Data[3] * 0.1f);
    }
  }

  OLED_PrintString((OLED_COLUMN - 12 * 2 - 6 * 5) / 2, (OLED_ROW - 12 * 2) / 2 + Increase, "温度:", &font12x12, Mode);
  OLED_PrintFNum((OLED_COLUMN - 12 * 2 - 6 * 5) / 2 + 12 * 2 + 6, (OLED_ROW - 12 * 2) / 2 + Increase, Tempertature, &font12x12, Mode);
  OLED_PrintString((OLED_COLUMN - 12 * 2 - 6 * 5) / 2, (OLED_ROW - 12 * 2) / 2 + 12 + Increase, "湿度:", &font12x12, Mode);
  OLED_PrintFNum((OLED_COLUMN - 12 * 2 - 6 * 5) / 2 + 12 * 2 + 6, (OLED_ROW - 12 * 2) / 2 + 12 + Increase, Humidity, &font12x12, Mode);
  OLED_PrintString(OLED_COLUMN - 6 * 4, OLED_ROW - 8 + Increase, "Back", &font8x6, Mode);
}
