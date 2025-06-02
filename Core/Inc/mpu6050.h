#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "main.h"
#include "i2c.h"
#include "cmsis_os.h"

void MPU6050_INIT(void);
void MPU6050_Read(void);

#endif //__MPU6050_H__
