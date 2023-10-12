/*
 * main.c
 *
 *  Created on: Jun 24, 2020
 *      Author: SOUAD
 */
#include "../Lib/Std_types.h"
#include "../Lib/Bit_math.h"

#undef F_CPU
#define F_CPU 8000000
#include <util/delay.h>

#include "Uart_int.h"
#include "Lcd_int.h"

#define KEYBOARD_ARROW_KEY     0x1B
#define KEYBOARD_ENTER_KEY     0x0D
#define KEYBOARD_BACKSPACE_KEY 0x7F
#define KEYBOARD_DELETE_KEY     0x7E

int main(void)
{
	/* Var to receive data via UART  */
	u8 App_u8RecievedData ;
	/* 2 variables to set the location of the char*/
	u8 Lcd_u8RawPos ;
	u8 Lcd_u8ColumnPos ;
	Lcd_u8RawPos = LCD_1STRAW ;
	Lcd_u8ColumnPos = LCD_1STCOLUMN ;
	/* Initialize UART */
	Uart_vidInit();
	/* Initialize LCD */
	Lcd_vidInit();

	while(1)
	{
		/* set the cursor position */
		if((Lcd_u8ColumnPos >= 0)&&(Lcd_u8ColumnPos <= 15))
		{
			Lcd_u8RawPos = LCD_1STRAW ;
			Lcd_vidSendCommand(LCD_u8CMD_BEGIN_CURS_1ST_RAW+Lcd_u8ColumnPos);
			_delay_ms(2);
		}
		else if((Lcd_u8ColumnPos >= 16)&&(Lcd_u8ColumnPos <= 31))
		{
			Lcd_u8RawPos = LCD_2NDRAW ;
			Lcd_vidSendCommand(LCD_u8CMD_BEGIN_CURS_2ND_RAW+((Lcd_u8ColumnPos)-16));
			_delay_ms(2);
		}
		else if(Lcd_u8ColumnPos > 31)
		{
			Lcd_u8RawPos = LCD_1STRAW ;
			Lcd_u8ColumnPos = LCD_1STCOLUMN ;
			Lcd_vidSendCommand(LCD_u8CMD_BEGIN_CURS_1ST_RAW);
			_delay_ms(2);
		}
		else
		{
			Lcd_u8RawPos = LCD_1STRAW ;
			Lcd_u8ColumnPos = LCD_1STCOLUMN ;
			Lcd_vidSendCommand(LCD_u8CMD_BEGIN_CURS_1ST_RAW);
			_delay_ms(2);
		}

		/* Receive data from UART*/
		App_u8RecievedData = Uart_vidReceiveByte();


		/* if Arrow keys */
		if(App_u8RecievedData == KEYBOARD_ARROW_KEY )
		{
			App_u8RecievedData = Uart_vidReceiveByte();
			if(App_u8RecievedData == '[' )
			{
				App_u8RecievedData = Uart_vidReceiveByte();
				switch(App_u8RecievedData)
				{
				/* up Arrow key */
				case 'A' :
				{
					if(Lcd_u8RawPos == LCD_2NDRAW )
					{
						Lcd_u8ColumnPos = Lcd_u8ColumnPos-16;
					}
					else if(Lcd_u8RawPos == LCD_1STRAW )
					{
					}
					break ;
				}
				/* down Arrow key */
				case 'B' :
				{
					if(Lcd_u8RawPos == LCD_1STRAW )
					{
						Lcd_u8ColumnPos = Lcd_u8ColumnPos+16;
					}
					else if(Lcd_u8RawPos == LCD_2NDRAW )
					{
					}
					break ;
				}
				/* right Arrow key */
				case 'C' :
				{
					Lcd_vidSendCommand(LCD_u8CMD_MOV_CURS_RIGHT);
					_delay_ms(2);
					Lcd_u8ColumnPos++;
					break ;
				}
				/* left Arrow key */
				case 'D' :
				{
					Lcd_vidSendCommand(LCD_u8CMD_MOV_CURS_LEFT);
					_delay_ms(2);
					Lcd_u8ColumnPos--;
					break ;
				}
				}
			}
		}


		/* if enter key */
		else if(App_u8RecievedData == KEYBOARD_ENTER_KEY )
		{
			Lcd_vidSendCommand(LCD_u8CMD_BEGIN_CURS_2ND_RAW);
			_delay_ms(2);
			Lcd_u8ColumnPos = 16;
		}

		/* if backspace key */
		else if(App_u8RecievedData == KEYBOARD_BACKSPACE_KEY )
		{
			if(Lcd_u8ColumnPos == 16)
			{
				Lcd_vidSendCommand(LCD_u8CMD_BEGIN_CURS_1ST_RAW+15);
				_delay_ms(2);
				Lcd_vidWriteChar(' ');
				Lcd_u8ColumnPos-- ;
			}
			else
			{
				Lcd_vidSendCommand(LCD_u8CMD_MOV_CURS_LEFT);
				_delay_ms(2);
				Lcd_vidWriteChar(' ');
				Lcd_u8ColumnPos--;
			}
		}

		/* delete key (Clear Display) */
		else if(App_u8RecievedData == KEYBOARD_DELETE_KEY )
		{
			Lcd_vidSendCommand(LCD_u8CMD_CLR_DISPLAY);
			_delay_ms(2);
			Lcd_u8ColumnPos = LCD_1STCOLUMN ;
		}

		/* Other keys */
		else
		{
			Lcd_vidWriteChar(App_u8RecievedData);
			Lcd_u8ColumnPos++;
			/* display on pc screen */
			Uart_vidSendByte(App_u8RecievedData);
			Uart_vidSendByte('\r');
			Uart_vidSendByte('\n');
		}
	}
	return 0;
}

