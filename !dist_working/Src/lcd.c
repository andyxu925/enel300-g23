#include "lcd.h"

static I2C_HandleTypeDef *lcd_i2c;

#define LCD_ADDR (0x27 << 1)
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_CMD 0x00
#define LCD_DATA 0x01

static void lcd_write(uint8_t data, uint8_t mode)
{
    uint8_t high = data & 0xf0;
    uint8_t low = (data << 4) & 0xf0;

    uint8_t sequence[4] = {0};
    sequence[0] = high | mode | LCD_BACKLIGHT | LCD_ENABLE;
    sequence[1] = high | mode | LCD_BACKLIGHT;
    sequence[2] = low | mode | LCD_BACKLIGHT | LCD_ENABLE;
    sequence[3] = low | mode | LCD_BACKLIGHT;

    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR, sequence, 4, HAL_MAX_DELAY);
}

void lcd_cmd(uint8_t cmd)
{
    lcd_write(cmd, LCD_CMD);
}

void lcd_data(uint8_t data)
{
    lcd_write(data, LCD_DATA);
}

void lcd_clear(void)
{
    lcd_cmd(0x01);
    HAL_Delay(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xc0 + col);
    lcd_cmd(addr);
}

void lcd_str(char *str)
{
    while (*str)
    {
        lcd_data(*str++);
    }
}

void lcd_init(I2C_HandleTypeDef *hi2c1)
{
    lcd_i2c = hi2c1;
    HAL_Delay(50);
    lcd_cmd(0x33);
    lcd_cmd(0x32);
    lcd_cmd(0x28);
    lcd_cmd(0x0c);
    lcd_cmd(0x06);
    lcd_clear();
}