/* Includes io.h header file where all the Input/Output Registers and its Bits are defined for all AVR microcontrollers */
#include <avr/io.h>
/* Defines a macro for the delay.h header file. F_CPU is the microcontroller frequency value for the delay.h header file */
#define	F_CPU	8000000
/* Includes delay.h header file which defines two functions, _delay_ms (millisecond delay) and _delay_us (microsecond delay) */
#include <util/delay.h>
#include <stdlib.h>
/* Includes adc.h header file which defines different functions for Analog to Digital Converter */
#include "adc.h"
#include "lcd.h"

/* Variable declarations */
uint8_t data;
uint8_t counter = 0;
uint16_t ch2_ref;
uint16_t ch3_ref;
uint16_t ch2_read;
uint16_t ch3_read;
uint16_t ADC_ch2_value;
uint16_t ADC_ch3_value;
char num_in[20];
char num_out[20];
uint8_t inFlag = 0;
uint8_t outFlag = 0;

/* Function declarations */
void ADC_ch2();
void ADC_ch3();

int main(void)
{
	DDRD = 0b00110000;				/* Configure PortD as output */
	PORTD = 0x00;					/* Set LOW value to PORT D */
	DDRA = 0x00;					/* Configure PortA as input */
	
	adc_init();					/* ADC initialization */
	lcd_init(LCD_DISP_ON);
	
	lcd_gotoxy(0,0);				/* Set cursor to 1. row (row0) and 1. character */
	lcd_puts("Dobrodosli");
	_delay_ms(1000);
	
	ch2_read = read_adc_channel(2);			/* Converting analog output of Digital light sensor */
	ch2_ref = ch2_read;

	ch3_read = read_adc_channel(3);			/* Converting analog output of Digital light sensor */
	ch3_ref = ch3_read;

	/* Start of infinite loop */
	while (1)
	{
		ADC_ch2();
		ADC_ch3();
		
		if (ADC_ch2_value < (ch2_ref - 100))
		{
			outFlag = 1;
			
			if (inFlag == 1)
			{
				counter++;
				PORTD |= (1<<PD5);	/* Turn ON LED on PD5 */
				inFlag = 0;
				outFlag = 0;
				_delay_ms(200);
				lcd_gotoxy(0,0);
				lcd_puts("Direction: -->");
				
			}
			else
			{
				counter = counter;
				lcd_gotoxy(0,0);
				lcd_puts("Direction:    ");	
			}
			sprintf(num_in, "Visitors: %d ", counter);
			lcd_gotoxy(0,1);
			lcd_puts(num_in);
		}
		
		else if (ADC_ch3_value < (ch3_ref - 100))
		{
			inFlag = 1;
			
			if (outFlag == 1)
			{
				if (counter > 0)
				{
					counter--;
				}
				else if (counter == 0)
				{
					counter = 0;
				}
				
				inFlag = 0;
				outFlag = 0;
				PORTD |= (1<<PD4);	/* Turn ON LED on PD4 */
				_delay_ms(200);
				lcd_gotoxy(0,0);
				lcd_puts("Direction: <--");

			}
			else
			{
				counter = counter;
				lcd_gotoxy(0,0);
				lcd_puts("Direction:    ");
				
			}
			sprintf(num_in, "Visitors: %d ", counter);
			lcd_gotoxy(0,1);
			lcd_puts(num_in);
		}
		else
		{
			PORTD = 0x00;
			counter = counter;	
		}			
	}
	return 0;
}

void ADC_ch2()
{
	ADC_ch2_value = read_adc_channel(2);		/* Converting analog output of Digital light sensor */	
}

void ADC_ch3()
{
	ADC_ch3_value = read_adc_channel(3);		/* Converting analog output of Digital light sensor*/
}
