/*
 * SPIM02.c
 *
 * Created: 2/4/2026 11:07:32 PM
 * Author : Eliph
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "SPIC/SPIC.h"
#include "USART/USART.h"

//GLOBAL: USART.
uint8_t M1_STORE_FLAG = 0x00;
uint8_t CN;
char MI;
uint8_t US_VALUES[4] = {0};
volatile uint8_t NUM[3] = {0};
volatile uint8_t CNT = 0x00;
volatile uint8_t FR = 0x00;
uint8_t RS;


void SETUP (void){
	cli();
	
	//TIMERS: 16 prescaler.
	CLKPR = (1 << CLKPCE);
	CLKPR = (1 << CLKPS2);
	
	//PORTD as output:
	DDRD = 0xFF;
	DDRB = 0xFF;
	
	//SPI: MASTER.
	SPI_SETUP(SPI_MASTER, SPI_MODE0,SPI_MSB,128);
	
	//CLEAR:
	PORTD &= 0x00;
	
	//USART:
	AS_USART(9600,1,1,1,1,8);
	
	sei();
}

//ISR:
ISR(USART_RX_vect){
	//RECIVE: COMPLETE.
	uint8_t ST = UDR0;
	
	if(CNT == 0x00){
		NUM[0] = ST;
		CNT++;
	}
	else if(CNT == 0x01){
		NUM[1] = ST;
		CNT++;
	}
	else if(CNT == 0x02){
		NUM[2] = ST;
		CNT++;
	}
	
	if(CNT == 0x03){
		CNT = 0x00;
		FR = 0x01;
	}
	else{}	
}

int main(void)
{
	//SETUP:
    SETUP();
	
    while (1) 
    {
		//PROCESSING:
		if(FR == 0x01){
			RS = USART_ASCII_TO_U8(NUM);
			FR = 0x00;
			USART_TR("_______________________________");
			USART_TR("\r\n");
			USART_TR_ADC(RS);
			USART_ADC_OUT();
			USART_TR("\r\n");
			PORTD &= 0x00;
			PORTB &= 0b001111;
			// PORTD: bits 2..7
			PORTD = (PORTD & 0x03) | (RS & 0xFC);

			// PORTB: bits 0..1
			PORTB = (PORTB & 0xFC) | (RS & 0x03);
			
			//TRANSMIT: RS.
			USART_TR("\r\n");
			USART_TR("Transmitting data...");
			USART_TR("\r\n");
			
			PORTB &= ~(1<<PORTB2); // Select slave
			SPI_MST_Transmit(RS);
			PORTB |= (1<<PORTB2);  // Deselect slave
		}
		else{}
    }
}

