/*
 * SPIP02.c
 *
 * Created: 2/4/2026 11:10:15 PM
 * Author : Erick Javier Segura 231141 - Samuel Urbina 23434
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "SPIC/SPIC.h"
#include "USART/USART.h"

//SETUP:
void SETUP(void){
	cli();
	//TIMERS: 16 prescaler.
	CLKPR = (1 << CLKPCE);
	CLKPR = (1 << CLKPS2);
	
	//OUTPUT LEDs:
	DDRD = 0xFF;
	DDRB = 0xFF;
	
	//PORTC as input:
	DDRC = 0x00;
	
	//SPI: SLAVE.
	SPI_SETUP(SPI_SLAVE, SPI_MODE0,SPI_MSB,128);
	
	//
	SPDR = 0x00;
	
	//ENABLE: SPI_ISR.
	SPCR |= (1<<SPIE);
	
	sei();
}

//ISR:
ISR(SPI_STC_vect){
	//READ:
	uint8_t SPI_VALUE = SPDR;
	
	//OUT:
	PORTD &= 0x00;
	PORTB &= 0b001111;
	PORTD = (PORTD & 0x03) | (SPI_VALUE & 0xFC);

	PORTB = (PORTB & 0xFC) | (SPI_VALUE & 0x03);
}

int main(void)
{
    //SETUP:
    SETUP();
	
	//USART:
	AS_USART(9600,1,1,1,1,8);
	
    while (1) 
    {
    }
}

