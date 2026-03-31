#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"

void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_str(char *str);
void lcd_init(I2C_HandleTypeDef *lcd_i2c);

#endif /* INC_LCD_H_ */