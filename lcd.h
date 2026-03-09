/*
 * lcd.h
 *
 *  Created on: Mar 6, 2026
 *      Author: diana
 */

#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include "stm32f4xx_hal.h"
#include <string.h>
#include "main.h"

#define LCD_ADDR (0x27 << 1)

void LCD_Init(I2C_HandleTypeDef* hi2c);
void LCD_Send_String(char* str);
void LCD_Set_Cursor(uint8_t row, uint8_t col);
void LCD_Clear(void);
void LCD_Send_Cmd(uint8_t cmd);
void LCD_Send_Data(uint8_t data);

#endif
