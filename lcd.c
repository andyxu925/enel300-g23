/*
 * lcd.c
 *
 *  Created on: Mar 6, 2026
 *      Author: diana
 */


#include "lcd.h"

static I2C_HandleTypeDef* lcd_i2c;

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_CMD       0x00
#define LCD_DATA      0x01

static void LCD_Send_4Bits(uint8_t data) {
    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR, &data, 1, HAL_MAX_DELAY);
    HAL_Delay(1);
}

static void LCD_Write(uint8_t data, uint8_t mode) {
    uint8_t high = data & 0xF0;
    uint8_t low = (data << 4) & 0xF0;

    uint8_t sequence[4];
    sequence[0] = high | mode | LCD_BACKLIGHT | LCD_ENABLE;
    sequence[1] = high | mode | LCD_BACKLIGHT;
    sequence[2] = low | mode | LCD_BACKLIGHT | LCD_ENABLE;
    sequence[3] = low | mode | LCD_BACKLIGHT;

    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR, sequence, 4, HAL_MAX_DELAY);
}

void LCD_Send_Cmd(uint8_t cmd) {
    LCD_Write(cmd, LCD_CMD);
}

void LCD_Send_Data(uint8_t data) {
    LCD_Write(data, LCD_DATA);
}

void LCD_Clear(void) {
    LCD_Send_Cmd(0x01);
    HAL_Delay(2);
}

void LCD_Set_Cursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Send_Cmd(addr);
}

void LCD_Send_String(char* str) {
    while (*str) {
        LCD_Send_Data(*str++);
    }
}

void LCD_Init(I2C_HandleTypeDef* hi2c) {
    lcd_i2c = hi2c;

    HAL_Delay(50);
    LCD_Send_Cmd(0x33);
    LCD_Send_Cmd(0x32);
    LCD_Send_Cmd(0x28);
    LCD_Send_Cmd(0x0C);
    LCD_Send_Cmd(0x06);
    LCD_Clear();
}
