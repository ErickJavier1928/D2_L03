/*
 * SPI.c
 *
 * Created: 1/28/2026 9:55:15 PM
 * Author : Eliph
 */ 

#define F_CPU 1000000
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "SPIC/SPIC.h"
#include "USART/USART.h"

//GLOBAL:
uint8_t NT = 0x00;
uint8_t ADC_PC0 = 0x00;
uint8_t ADC_PC1 = 0x00;

//USART:
uint8_t M1_STORE_FLAG = 0x00;
uint8_t CN;
char MI;
uint8_t US_VALUES[4] = {0};

void SETUP(void){
	cli();
	
	//TIMERS: 16 prescaler.
	CLKPR = (1 << CLKPCE);
	CLKPR = (1 << CLKPS2);
	
	//OUTPUT LEDs:
	DDRD = 0xFF;
	
	//SPI: MASTER.
	SPI_SETUP(SPI_MASTER, SPI_MODE0,SPI_MSB,128);
	
	//CLEAR:
	PORTD &= 0x00;
	
	//USART:
	AS_USART(9600,1,1,1,1,8);
	
	
	sei();
}

int main(void)
{
    //SETUP
	SETUP();
	
	
    while (1) 
    {
		PORTB &= ~(1<<PORTB2); // Select slave
		
		//Ask for first ADC value: PC0.
		SPI_MST_Transmit('S');
		NT = SPDR;
		SPI_MST_Transmit(0x00);
		//RECIVE:
		ADC_PC0 = SPDR;
		
		//Ask for first ADC value: PC1.
		SPI_MST_Transmit('D');
		NT = SPDR;
		SPI_MST_Transmit(0x00);
		//RECIVE:
		ADC_PC1 = SPDR;
		
		PORTB |= (1<<PORTB2);  // Deselect slave
		
		//POST:
		//PORTD = 0x00;
		//PORTD = ADC_PC0;
		USART_TR("_______________________________");
		USART_TR("\r\n");
		USART_TR("ADC -> PC0:");
		USART_TR_ADC(ADC_PC0);
		USART_ADC_OUT();
		USART_TR("\r\n");
	
	
		USART_TR("\r\n");
		USART_TR("ADC -> PC1:");
		USART_TR_ADC(ADC_PC1);
		USART_ADC_OUT();
		USART_TR("\r\n");
		
		USART_TR("_______________________________");
	
		//WAIT:
		//_delay_ms(500);
		
    }
}

