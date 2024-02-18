
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "DIO.h"
#include "LCD.h"
#include "std_macros.h"
#include <math.h>
#include <stdio.h>
int main(void)
{
	LCD_vInit();
	DIO_vsetPINDir('D', 3, 1);
	DIO_vsetPINDir('A', 2, 1);
	unsigned long a, b, high, distance;
	while (1)
	{
		TCCR1A = 0;
		TIFR |= (1 << ICF1); // Clear ICF (Input Capture flag) 
		DIO_write('D', 3, 1); // Create pulse for 50 ms
		_delay_ms(50);
		DIO_write('D', 3, 0);
		TCCR1B = 0xc1;  // Rising edge, no prescaler , noise canceler
		while(IS_BIT_CLR(TIFR, ICF1));
		a = ICR1;
		TIFR |= (1 << ICF1);
		TCCR1B = 0x81;  // Falling edge, no prescaler ,noise canceler
		while(IS_BIT_CLR(TIFR, ICF1));
		b = ICR1;
		TIFR |= (1 << ICF1);
		TCNT1 = 0;
		TCCR1B = 0;    // Stop the timer
		high = b - a;
		distance = ((34600 * high) / (F_CPU * 2));
		if (distance>=80)
		{
			LCD_clearscreen();
			LCD_vSend_string("No object");
			_delay_ms(500);
			
		}
		else
		{
			char buf[5];
			LCD_sendFloat(distance, buf, 0);
			LCD_clearscreen();
			LCD_vSend_string("Distance = ");
			LCD_vSend_string(buf);
			LCD_vSend_string("cm");
			_delay_ms(500);
			
			if(distance <= 5){       //  There is an obstacle coming
			    DIO_write('A',2,1);  // buzzer start
			}
			else if(distance <= 10){
				DIO_write('A',2,1);   // buzzer start for 10 ms for attention 
				_delay_ms(50);
				DIO_write('A',2,0);
				_delay_ms(50);
			}
			else {
				DIO_write('A',2,0);
			}
			
		}			
		
	}
}   

