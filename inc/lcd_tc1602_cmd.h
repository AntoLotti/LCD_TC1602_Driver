/*
 * lcd_tc1602_cmd.h
 *
 *  Created on: Apr 1, 2026
 *      Author: lotti
 */

#ifndef INC_LCD_TC1602_CMD_H_
#define INC_LCD_TC1602_CMD_H_

/*
*	bit 7  bit 6   bit 5  bit 4  bit 3  bit 2   bit 1  bit 0
* 	D7      D6     D5     D4     D3     D2      D1     D0
*/ 

#define CMD_CLEAR_DISPLAY_VALUE			(uint8_t)(0x01)
#define CMD_RETURN_HOME_VALUE			(uint8_t)(0x02)
#define CMD_ENTRY_MODE_1_VALUE			(uint8_t)(0x07)
#define CMD_ENTRY_MODE_2_VALUE			(uint8_t)(0x06)
#define CMD_ENTRY_MODE_3_VALUE			(uint8_t)(0x05)
#define CMD_ENTRY_MODE_4_VALUE			(uint8_t)(0x04)
#define CMD_DISPLAY_OFF_VALUE			(uint8_t)(0x08)
#define CMD_DISPLAY_MODE_1_VALUE		(uint8_t)(0x0C)	
#define CMD_DISPLAY_MODE_2_VALUE		(uint8_t)(0x0E)	
#define CMD_DISPLAY_MODE_3_VALUE		(uint8_t)(0x0F)	
#define CMD_FUNCTION_SET_MODE_1_VALUE	(uint8_t)(0x3C)
#define CMD_FUNCTION_SET_MODE_2_VALUE	(uint8_t)(0x38)
#define CMD_FUNCTION_SET_MODE_3_VALUE	(uint8_t)(0x34)
#define CMD_FUNCTION_SET_MODE_4_VALUE	(uint8_t)(0x30)
#define CMD_FUNCTION_SET_MODE_5_VALUE	(uint8_t)(0x2C)
#define CMD_FUNCTION_SET_MODE_6_VALUE	(uint8_t)(0x28)
#define CMD_FUNCTION_SET_MODE_7_VALUE	(uint8_t)(0x24)
#define CMD_FUNCTION_SET_MODE_8_VALUE	(uint8_t)(0x20)

#define SET_DDRAM_ADDRESS	(uint8_t)(0x80)

typedef enum cmd_e
{
	CMD_CLEAR_DISPLAY			= 0,
	CMD_RETURN_HOME				= 1,
	CMD_ENTRY_MODE_1			= 2,	// Increment cursor after each write & Shift display when writing
	CMD_ENTRY_MODE_2			= 3,	// Increment cursor after each write & NO  Shift display when writing
	CMD_ENTRY_MODE_3			= 4,	// Decrement cursor after each write & Shift display when writing
	CMD_ENTRY_MODE_4			= 5,	// Decrement cursor after each write & NO  Shift display when writing
	CMD_DISPLAY_OFF				= 6,
	CMD_DISPLAY_MODE_1			= 7,	// Display ON, Cursor OFF, Blink OFF.
	CMD_DISPLAY_MODE_2			= 8,	// Display ON, Cursor ON, Blink OFF.
	CMD_DISPLAY_MODE_3			= 9,	// Display ON, Cursor ON, Blink ON.
	CMD_FUNCTION_SET_MODE_1		= 10,	// 8 Bits; 2 Lines;	5x10 dots
	CMD_FUNCTION_SET_MODE_2		= 11,	// 8 Bits; 2 Lines;	5x8 dots
	CMD_FUNCTION_SET_MODE_3		= 12,	// 8 Bits; 1 Lines;	5x10 dots
	CMD_FUNCTION_SET_MODE_4		= 13,	// 8 Bits; 1 Lines;	5x8 dots
	CMD_FUNCTION_SET_MODE_5		= 14,	// 4 Bits; 2 Lines;	5x10 dots
	CMD_FUNCTION_SET_MODE_6		= 15,	// 4 Bits; 2 Lines;	5x8 dots
	CMD_FUNCTION_SET_MODE_7		= 16,	// 4 Bits; 1 Lines;	5x10 dots
	CMD_FUNCTION_SET_MODE_8		= 17,	// 4 Bits; 1 Lines;	5x8 dots
}cmd_t;

typedef enum mode_e
{
	CMD_MODE	= 0,
	DATA_MODE	= 1,
}mode_t;

#endif /* INC_LCD_TC1602_CMD_H_ */
