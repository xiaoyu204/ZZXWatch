#ifndef __OLED_H__
#define __OLED_H__

#include "font.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// OLED器件地址
#define OLED_ADDRESS       0x78

// OLED参数
#define OLED_PAGE          8             // OLED页数
#define OLED_ROW           8 * OLED_PAGE // OLED行数
#define OLED_COLUMN        128           // OLED列数

// OLED模式
#define OLED_MODE_NORMAL   0    // 正常模式 黑底白字
#define OLED_MODE_REVERSED 0xFF // 反色模式 白底黑字

#define OLED_I2C           hi2c2
#define OLED_TASK_BIT   (1 << 3)

typedef uint8_t       OLED_ColorMode;
extern OLED_ColorMode Mode;

void OLED_Init(void);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);

void OLED_NewFrame(uint8_t mode);
void OLED_ShowFrame(void);
void OLED_SetPixel(uint8_t x, uint8_t y, OLED_ColorMode color);
void OLED_SetBlock(uint8_t x, uint8_t y, const uint8_t *data, uint8_t w, uint8_t h,
                   OLED_ColorMode color);

void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_ColorMode color);
void OLED_DrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color);
void OLED_DrawFilledRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color);
void OLED_DrawTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3,
                       OLED_ColorMode color);
void OLED_DrawFilledTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3,
                             OLED_ColorMode color);
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color);
void OLED_DrawFilledCircle(uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color);
void OLED_DrawEllipse(uint8_t x, uint8_t y, uint8_t a, uint8_t b, OLED_ColorMode color);
void OLED_DrawImage(uint8_t x, uint8_t y, const Image *img, OLED_ColorMode color);

void OLED_PrintNum(uint8_t x, uint8_t y, uint32_t num, const Font *font, OLED_ColorMode color);
void OLED_PrintFNum(uint8_t x, uint8_t y, float Fnum, const Font *font, OLED_ColorMode color);
void OLED_PrintASCIIChar(uint8_t x, uint8_t y, char ch, const ASCIIFont *font,
                         OLED_ColorMode color);
void OLED_PrintASCIIString(uint8_t x, uint8_t y, char *str, const ASCIIFont *font,
                           OLED_ColorMode color);
void OLED_PrintString(uint8_t x, uint8_t y, char *str, const Font *font, OLED_ColorMode color);
void OLED_SendCmd(uint8_t cmd);

void OLED_Task(void *pvParameters);

#endif // __OLED_H__
