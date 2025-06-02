#ifndef __DHT11_H__
#define __DHT11_H__

#include "main.h"
#include "function.h"

#define DHT11_IO GPIOB
#define DHT11_PIN GPIO_PIN_0

void DHT_GPIO_INPUT(void);
void DHT_GPIO_OUTPUT(void);
void DHT11_Start(void);
uint8_t DHT_Read_Byte(void);
void DHT_Read(void);

#endif //__DHT11_H__
