/*
 * tempSensor1.c
 *
 * Created: 06-Jun-18 5:42:32 PM
 * Author : Ravindu
 */ 

#define F_CPU 1000000UL // 16 MHz clock speed


#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#define BIT0 0b00000001
#define BIT1 0b00000010
#define BIT2 0b00000100
#define BIT3 0b00001000
#define BIT4 0b00010000
#define BIT5 0b00100000
#define BIT6 0b01000000
#define BIT7 0b10000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "ds18b20.h"
#define UART_BAUD_RATE 2400
#include "uart.h"


int main(void)
{
    /* Replace with your application code */
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	DDRD = 0xFF;//output
	DDRB = 0xFF; //output
	DDRA = ~BIT1;
	DDRC = 0xFE;
	
	char printbuff[100];
	double d = 0;
	
	Lcd4_Init();

	sei();
	//PORTC |= BIT1;
	//PORTD |= BIT0;
    while (1) 
    {	
		
		if(!bit_is_set(PINA,1))
		{
			
		//1st motor
		PORTA = BIT2;
		 // 1 minute rotation
		_delay_ms(60000);
		PORTA = BIT3;
		 // 2 minutes rotation
		_delay_ms(120000);
		PORTA &= ~BIT3;

		//2nd motor
		PORTA = BIT4;
		 // 3 minutes rotation DUMMY VALUE
		_delay_ms(180000);
		PORTA &= ~BIT4;
		//1st motor reverse
		PORTA &= ~BIT2;
		_delay_ms(1000);

		//piston and cutting part
		
		//repeating motor 3,4,5,6 FOUR TIMES
		for(int k=0;k<4;k++)
		{
			//3rd motor
			PORTA = BIT5;
		 // 2 minutes rotation
			_delay_ms(12000);
			PORTA &= ~BIT5;
			
			//4th motor (Cutting part)
			PORTB = BIT0;
			//5 seconds rotation
			_delay_ms(5000);
			PORTB = BIT1;
			//5 seconds reverse rotation
			_delay_ms(5000);
			
			//5th motor (belt)
			PORTB = BIT2 && BIT5;
			//10 seconds rotation
			_delay_ms(10000);
			PORTB = BIT3;
			//10 seconds rotation reverse
			_delay_ms(10000);
			PORTB &= ~BIT3 && ~BIT5;
			
			//6th motor
			PORTB = BIT4;
			//3 seconds rotation
			_delay_ms(3000);
			PORTB &= ~BIT4;
			
		}
		
		//motor 6
		PORTB = BIT4;
		//30 seconds rotation
		_delay_ms(30000);
		PORTB  = BIT5;
		//30 seconds rotation
		_delay_ms(30000);
		PORTB &= ~BIT5;
		
		int a=1;// 1 MINUTE FOR LCD AND TEMP SENSOR
	
	for(int i=0;i<a*60;i++){
		
		d = ds18b20_gettemp();
		
			if(d < 80)
			PORTD=(1<<PORTD1);
			if(d >100)
			PORTD &= ~BIT1;
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Temperature: ");
			Lcd4_Set_Cursor(2,0);
			dtostrf(d, 10, 3, printbuff);
			Lcd4_Write_String(printbuff);
			_delay_ms(1000);
			
		}
		Lcd4_Clear();
		break;
		}
    }
}

