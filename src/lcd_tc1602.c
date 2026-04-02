/*
 * lcd_tc1602.c
 *
 *  Created on: Mar 31, 2026
 *      Author: lotti
 */

#include "lcd_tc1602.h"

static void split_str_16(const char *src, char *first_half, char *second_half)
{
	if( src == NULL || first_half == NULL || second_half == NULL )
		return;

	uint8_t count = 0x00;
	while(count < 16 && *src != '\0')
	{
		*first_half++ = *src++;
		count++;
	}
	*(first_half) = '\0';

	while(*src != '\0')
	{
		*second_half++ = *src++;
	}
	*(second_half) = '\0';

}

static uint16_t count_char(const char *src)
{
	if(src == NULL)
		return 0x0000;

	uint16_t count = 0x0000;

	while(*src != '\0')
	{
		src++;
		count++;
	}

	return count;
}

static uint8_t get_cmd_val(cmd_t cmd)
{
	switch(cmd)
	{
		case CMD_CLEAR_DISPLAY:
			return CMD_CLEAR_DISPLAY_VALUE;
		case CMD_RETURN_HOME:
			return CMD_RETURN_HOME_VALUE;
		case CMD_ENTRY_MODE_1:
			return CMD_ENTRY_MODE_1_VALUE;
		case CMD_ENTRY_MODE_2:
			return CMD_ENTRY_MODE_2_VALUE;
		case CMD_ENTRY_MODE_3:
			return CMD_ENTRY_MODE_3_VALUE;
		case CMD_ENTRY_MODE_4:
			return CMD_ENTRY_MODE_4_VALUE;
		case CMD_DISPLAY_OFF:
			return CMD_DISPLAY_OFF_VALUE;
		case CMD_DISPLAY_MODE_1:
			return CMD_DISPLAY_MODE_1_VALUE;
		case CMD_DISPLAY_MODE_2:
			return CMD_DISPLAY_MODE_2_VALUE;
		case CMD_DISPLAY_MODE_3:
			return CMD_DISPLAY_MODE_3_VALUE;
		case CMD_FUNCTION_SET_MODE_1:
			return CMD_FUNCTION_SET_MODE_1_VALUE;
		case CMD_FUNCTION_SET_MODE_2:
			return CMD_FUNCTION_SET_MODE_2_VALUE;
		case CMD_FUNCTION_SET_MODE_3:
			return CMD_FUNCTION_SET_MODE_3_VALUE;
		case CMD_FUNCTION_SET_MODE_4:
			return CMD_FUNCTION_SET_MODE_4_VALUE;
		case CMD_FUNCTION_SET_MODE_5:
			return CMD_FUNCTION_SET_MODE_5_VALUE;
		case CMD_FUNCTION_SET_MODE_6:
			return CMD_FUNCTION_SET_MODE_6_VALUE;
		case CMD_FUNCTION_SET_MODE_7:
			return CMD_FUNCTION_SET_MODE_7_VALUE;
		case CMD_FUNCTION_SET_MODE_8:
			return CMD_FUNCTION_SET_MODE_8_VALUE;
		
		default:
			return 0;
	}
}

static HAL_StatusTypeDef lcd_tc1602_i2c_send(I2C_HandleTypeDef *hi2c, uint8_t *data, uint16_t size)
{
	if(hi2c == NULL || data == NULL)
		return HAL_ERROR;

	return HAL_I2C_Master_Transmit(hi2c, SLAVE_ADD_W, data, size, TIME_OUT);
}

static HAL_StatusTypeDef lcd_tc1602_send(I2C_HandleTypeDef *hi2c, uint8_t src, uint8_t mode)
{
	/**
	 * DATA FORMAT:
	 * ----------------
	 * bit 7  	bit 6	bit 5	bit 4 	bit 3  	bit 2 	bit 1	bit 0
	 * D7 		D6 		D5 		D4 		D3 		D2  	D1  	D0
	 *
	 *
	 * LCD DATA FORMAT:
	 * ----------------
	 * bit 7  	bit 6	bit 5	bit 4 	bit 3  	bit 2 	bit 1	bit 0
	 * D7 		D6 		D5 		D4 		BL 		E  		RW  	RS
	 *
	 * X		X		X		X		1		1		0		0		<-- First Sent Of a Nibble
	 * X		X		X		X		1		0		0		0		<-- Second Sent Of a Nibble
	 *
	 */
	if(hi2c == NULL)
		return HAL_ERROR;

	uint8_t data_h = (uint8_t)(src & HIGH_DATA_MASK);
	uint8_t data_l = (uint8_t)((src & LOW_DATA_MASK) << SHIFT_LOW_BITS);

	uint8_t data[NUM_DATA] = {0x00};

	uint8_t mask_first	= (mode == CMD_MODE) ? CMD_MASK_FIRST  : DATA_MASK_FIRST;
	uint8_t mask_second	= (mode == CMD_MODE) ? CMD_MASK_SECOND : DATA_MASK_SECOND;

	// First Nibble (MOST SIGNIFICATIVE BITS)
	data[0]	= data_h | mask_first;
	data[1]	= data_h | mask_second;

	//Second Nibble (LEST SIGNIFICATIVE BITS)
	data[2]	= data_l | mask_first;
	data[3]	= data_l | mask_second;

	return lcd_tc1602_i2c_send(hi2c, data, NUM_DATA);
}


HAL_StatusTypeDef lcd_tc1602_send_cmd(I2C_HandleTypeDef *hi2c, uint8_t cmd)
{

	if(hi2c == NULL)
		return HAL_ERROR;

	if( !((cmd & SET_DDRAM_ADDRESS) == SET_DDRAM_ADDRESS))
		cmd = get_cmd_val(cmd);

	return lcd_tc1602_send(hi2c, cmd, CMD_MODE);
}


HAL_StatusTypeDef lcd_tc1602_send_data(I2C_HandleTypeDef *hi2c, uint8_t data)
{

	if(hi2c == NULL)
		return HAL_ERROR;

	return lcd_tc1602_send(hi2c, data, DATA_MODE);
}


HAL_StatusTypeDef lcd_tc1602_set_cursor(I2C_HandleTypeDef *hi2c, lcd_row_t row, uint8_t col)
{
	if(hi2c == NULL)
		return HAL_ERROR;

    switch (row)
    {
        case FIRST_ROW:
            col |= 0x80;
            break;
        case SECOND_ROW:
            col |= 0xC0;
            break;
    }

    return lcd_tc1602_send_cmd(hi2c, col);
}


HAL_StatusTypeDef lcd_tc1602_init(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == NULL)
		return HAL_ERROR;

	HAL_StatusTypeDef status = HAL_OK;

	// 4 bit initialization
	HAL_Delay(50);  			// wait for >40ms
	status = lcd_tc1602_send_cmd(hi2c, 0x30);
	if(status != HAL_OK)
		return status;

	HAL_Delay(5);  				// wait for >4.1ms
	status = lcd_tc1602_send_cmd(hi2c, 0x30);
	if(status != HAL_OK)
		return status;

	HAL_Delay(1); 	 			// wait for >100us
	status = lcd_tc1602_send_cmd(hi2c, 0x30);
	if(status != HAL_OK)
		return status;

	HAL_Delay(10);
	lcd_tc1602_send_cmd(hi2c, 0x20);  // 4bit mode
	if(status != HAL_OK)
		return status;

	// Display initialization
	HAL_Delay(10);
	status = lcd_tc1602_send_cmd(hi2c, CMD_FUNCTION_SET_MODE_6);
	if(status != HAL_OK)
		return status;

	HAL_Delay(10);
	status = lcd_tc1602_send_cmd(hi2c, CMD_DISPLAY_OFF);
	if(status != HAL_OK)
		return status;

	HAL_Delay(10);
	status = lcd_tc1602_send_cmd(hi2c, CMD_CLEAR_DISPLAY);
	if(status != HAL_OK)
		return status;

	HAL_Delay(20);
	status = lcd_tc1602_send_cmd(hi2c, CMD_ENTRY_MODE_2);
	if(status != HAL_OK)
		return status;

	HAL_Delay(10);
	status = lcd_tc1602_send_cmd(hi2c, CMD_DISPLAY_MODE_1);
	if(status != HAL_OK)
		return status;

	HAL_Delay(10);
	status = lcd_tc1602_send_cmd(hi2c, CMD_CLEAR_DISPLAY);
	if(status != HAL_OK)
		return status;

	HAL_Delay(1000);

	return status;
}


HAL_StatusTypeDef lcd_tc1602_print_Fr(I2C_HandleTypeDef *hi2c, char *src)
{
	HAL_StatusTypeDef status = HAL_OK;

	//Point to First Row Start
	status = lcd_tc1602_set_cursor(hi2c, FIRST_ROW, 0x00);
	while(*src)
	{
		status = lcd_tc1602_send_data(hi2c, *src++);
	}

	return status;
}


HAL_StatusTypeDef lcd_tc1602_print_Sr(I2C_HandleTypeDef *hi2c, char *src)
{
	HAL_StatusTypeDef status = HAL_OK;

	//Point to First Row Start
	status = lcd_tc1602_set_cursor(hi2c, SECOND_ROW, 0x00);
	while(*src)
	{
		status = lcd_tc1602_send_data(hi2c, *src++);
	}

	return status;
}


HAL_StatusTypeDef lcd_tc1602_print(I2C_HandleTypeDef *hi2c, char *src)
{
	if(hi2c == NULL || src == NULL)
		return HAL_ERROR;

	HAL_StatusTypeDef status  = HAL_OK;

	status = lcd_tc1602_send_cmd(hi2c, 0x01);
	HAL_Delay(3);
	if(status != HAL_OK)
		return status;

	uint16_t count = count_char(src);
	if(count > 32)
		return HAL_ERROR;

	if(count > 16)
	{
		char *first	= (char*)malloc(17 * sizeof(char));
		char *second = (char*)malloc(17 * sizeof(char));

		split_str_16(src, first, second);

		status = lcd_tc1602_print_Fr(hi2c, first);
		if(status != HAL_OK)
		{
			free(first);
			free(second);
			return status;
		}

		status = lcd_tc1602_print_Sr(hi2c, second);

		free(first);
		free(second);

	}
	else
	{
		status = lcd_tc1602_print_Fr(hi2c, src);
		if(status != HAL_OK)
			return status;
	}


	return status;
}





