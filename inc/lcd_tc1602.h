/*
 * lcd_tc1602.h
 *
 *  Created on: Mar 31, 2026
 *      Author: lotti
 */

#ifndef INC_LCD_TC1602_H_
#define INC_LCD_TC1602_H_


#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "stm32f4xx_hal.h"
#include "lcd_tc1602_cmd.h"


#define SLAVE_ADD_R		0x4F	//0100 A2 A1 A0 R
#define SLAVE_ADD_W		0x4E	//0100 A2 A1 A0 W

#define TIME_OUT		100		// I2C time out

#define NUM_DATA		(uint16_t)(4)	// Number of element to sent by I2C

#define HIGH_DATA_MASK	0xF0	// Mask to extract the most significant bit of a command
#define LOW_DATA_MASK	0x0F	// Mask to extract the less significant bit of a command

#define SHIFT_LOW_BITS		0x04

#define CMD_MASK_FIRST		(uint8_t)(0x0C)
#define CMD_MASK_SECOND		(uint8_t)(0x08)
#define DATA_MASK_FIRST		(uint8_t)(0x0D)
#define DATA_MASK_SECOND	(uint8_t)(0x09)

/* DD RAM Characters Positions */
#define LINE1_CHAR(pos) (uint8_t)(0x80 | pos)
#define LINE2_CHAR(pos) (uint8_t)(0xC0 | pos)

typedef enum lcd_row_e
{
	FIRST_ROW = 0,
	SECOND_ROW = 1,
}lcd_row_t;



HAL_StatusTypeDef lcd_tc1602_send_cmd(I2C_HandleTypeDef *hi2c, uint8_t cmd);

HAL_StatusTypeDef lcd_tc1602_send_data(I2C_HandleTypeDef *hi2c, uint8_t data);

HAL_StatusTypeDef lcd_tc1602_set_cursor(I2C_HandleTypeDef *hi2c, lcd_row_t row, uint8_t col);

HAL_StatusTypeDef lcd_tc1602_init(I2C_HandleTypeDef *hi2c);

HAL_StatusTypeDef lcd_tc1602_print_Fr(I2C_HandleTypeDef *hi2c, char *src);

HAL_StatusTypeDef lcd_tc1602_print_Sr(I2C_HandleTypeDef *hi2c, char *src);

HAL_StatusTypeDef lcd_tc1602_print(I2C_HandleTypeDef *hi2c, char *src);

#endif /* INC_LCD_TC1602_H_ */
