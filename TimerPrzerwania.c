/*
 * GccApplication1.c
 *
 * Created: 2013-01-21 12:36:11
 *  Author: student
 */ 




#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t licz;
int wart;
uint8_t numbers[10];

int main(void)
{
    int i = 0;
    numbers[0] = 0b11000000;
    numbers[1] = 0b11111001;
    numbers[2] = 0b10100100;
    numbers[3] = 0b10110000;
    numbers[4] = 0b10011001;
	numbers[5] = 0b10010010;
	numbers[6] = 0b10000010;
	numbers[7] = 0b11111000;
	numbers[8] = 0b10000000;
	numbers[9] = 0b10010000; 
   
   
    DDRA = 0xFF;
    DDRC = 0xFF;
    PORTA = 0x01;
   
	PORTC = 0x00;
	TCCR0 |= (1 << CS02);
	TIMSK |= (1 << TOIE0);
	sei();
   
   
    while(1)
    {
        
        /*
		PORTC = 0xFF;
        PORTA = numbers[(i)%10];
        PORTC = 0xFE;
        _delay_ms(2);
		
		if(i>=10)
		{
			PORTC = 0xFF;
			PORTA = numbers[((i)%100)/10];
			PORTC = 0xFD;
		}
		_delay_ms(2);
			
        if(i>=100)
        {
			PORTC = 0xFF;
			PORTA = numbers[((i)%1000)/100];
			PORTC = 0xFB;
		}    
		_delay_ms(2);
		if(i>=1000)
		{    
			PORTC = 0xFF;
			PORTA = numbers[((i)%10000)/1000];
			PORTC = 0xF7;
		}		
		_delay_ms(2);
        i++;
        if(i>=10000)
        {
	        i=0;
        }
		*/

       
       
/*            _delay_ms(250);
            PORTA = PORTA<<1;
            if(PORTA == 0)
                PORTA = 0x01;        */
    }
}

ISR(TIMER0_OVF_vect)
{
	if((licz%4) == 0)
	{
		wart++;
		PORTC = 0xFF;
		PORTA = numbers[wart%10];
		PORTC = 0xFE;
	}else if(((licz%4) == 1)&&(wart>=10))
	{
		PORTC = 0xFF;
		PORTA = numbers[(wart%100)/10];
		PORTC = 0xFD;
	}else if(((licz%4) == 2)&&(wart>=100))
	{
		PORTC = 0xFF;
		PORTA = numbers[(wart%1000)/100];
		PORTC = 0xFB;
	}else if(wart>=1000)
	{
		PORTC = 0xFF;
		PORTA = numbers[(wart%10000)/1000];
		PORTC = 0xF7;
	}
	licz++;
	if(wart>=10000)
	{
		wart=0;
	}
}