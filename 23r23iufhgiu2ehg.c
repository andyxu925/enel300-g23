// main.c
#include "lcd.h"
#include "ultrasonic.h"
#include <stdio.h>

char addr[8] = {0};
for (int i = 0; i < 128; i++)
{
    if (HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 3, 5) == HAL_OK)
    {
        sprintf(addr, "0x%X\r\n", i);
        HAL_UART_Transmit(&huart2, addr, sizeof(addr), 100);
    }
}

// ultrasonic.h
#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"

void Ultrasonic_Init(void);
float Ultrasonic_Read(void);

#endif /* INC_ULTRASONIC_H_ */

// ultrasonic.c
#include "ultrasonic.h"

// ------------------------ DWT Microsecond Timer ------------------------
static void DWT_Init(void)
{
    // Enable DWT cycle counter
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0; // reset counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static uint32_t GetMicroseconds(void)
{
    return DWT->CYCCNT / (SystemCoreClock / 1000000);
}

static void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < ticks)
        ;
}

// ------------------------ Ultrasonic Functions ------------------------
void Ultrasonic_Init(void)
{
    // Initialize DWT timer for microseconds
    DWT_Init();
}

static void Ultrasonic_Trigger(void)
{
    // Send 10 µs pulse to TRIG
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

float Ultrasonic_Read(void)
{
    uint32_t start = 0, stop = 0;

    Ultrasonic_Trigger(); // Send pulse

    // Wait for ECHO to go HIGH
    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_RESET)
        ;

    start = GetMicroseconds();

    // Wait for ECHO to go LOW
    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_SET)
        ;

    stop = GetMicroseconds();

    uint32_t pulse_width = stop - start;                // µs
    float distance_cm = (pulse_width * 0.0343f) / 2.0f; // cm
    return distance_cm;
}

// lcd.h
#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include "stm32f4xx_hal.h"
#include <string.h>
#include "main.h"

#define LCD_ADDR (0x27 << 1)

void LCD_Init(I2C_HandleTypeDef *hi2c);
void LCD_Send_String(char *str);
void LCD_Set_Cursor(uint8_t row, uint8_t col);
void LCD_Clear(void);
void LCD_Send_Cmd(uint8_t cmd);
void LCD_Send_Data(uint8_t data);

#endif

// lcd.h
#include "lcd.h"

static I2C_HandleTypeDef *lcd_i2c;

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_CMD 0x00
#define LCD_DATA 0x01

static void LCD_Send_4Bits(uint8_t data)
{
    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR, &data, 1, HAL_MAX_DELAY);
    HAL_Delay(1);
}

static void LCD_Write(uint8_t data, uint8_t mode)
{
    uint8_t high = data & 0xF0;
    uint8_t low = (data << 4) & 0xF0;

    uint8_t sequence[4];
    sequence[0] = high | mode | LCD_BACKLIGHT | LCD_ENABLE;
    sequence[1] = high | mode | LCD_BACKLIGHT;
    sequence[2] = low | mode | LCD_BACKLIGHT | LCD_ENABLE;
    sequence[3] = low | mode | LCD_BACKLIGHT;

    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR, sequence, 4, HAL_MAX_DELAY);
}

void LCD_Send_Cmd(uint8_t cmd)
{
    LCD_Write(cmd, LCD_CMD);
}

void LCD_Send_Data(uint8_t data)
{
    LCD_Write(data, LCD_DATA);
}

void LCD_Clear(void)
{
    LCD_Send_Cmd(0x01);
    HAL_Delay(2);
}

void LCD_Set_Cursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Send_Cmd(addr);
}

void LCD_Send_String(char *str)
{
    while (*str)
    {
        LCD_Send_Data(*str++);
    }
}

void LCD_Init(I2C_HandleTypeDef *hi2c)
{
    lcd_i2c = hi2c;

    HAL_Delay(50);
    LCD_Send_Cmd(0x33);
    LCD_Send_Cmd(0x32);
    LCD_Send_Cmd(0x28);
    LCD_Send_Cmd(0x0C);
    LCD_Send_Cmd(0x06);
    LCD_Clear();
}