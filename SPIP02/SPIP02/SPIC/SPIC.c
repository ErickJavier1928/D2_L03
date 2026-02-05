/*
 * SPIC.c
 *
 * Created: 2/5/2026 1:16:44 AM
 *  Author: Eliph
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "SPIC.h"

void SPI_SETUP(SPI_Role RL, SPI_Mode MD,SPI_Data_Order DO, uint8_t FOSC){
	//DISABLE_SPI:
	SPCR &= ~(1<<SPE);
	
	//MASTER:
	if(RL == 0){
		//SS as output:
		DDRB |= (1<<DDB2);
		//PORTB |= (1<<PORTB2);
		
		//MOSI and SCK as output:
		DDRB |= (1<<DDB3)|(1<<DDB5);
		
		//MISO as input:
		DDRB &= ~(1<<DDB4);
		
		//MASTER_SELECT:
		SPCR |= (1<<MSTR);
		
	}
	
	//SLAVE:
	else if(RL == 1){
		
		//SS as input:
		DDRB &= ~(1<<DDB2);
		PORTB |= (1<<PORTB2);
		
		//SCK and MOSI as input:
		DDRB &= ~((1<<DDB3)|(1<<DDB5));
		
		//MISO as output:
		DDRB |= (1<<DDB4);
		
		//SLAVE_SELECT:
		SPCR &= ~(1<<MSTR);
		
	}
	else{}
	
	//GENERAL:
	
	//DATA_ORDER:
	if(DO == 0){
		//LSB:
		SPCR |= (1<<DORD);
	}
	else if (DO == 1){
		//MSB:
		SPCR &= ~(1<<DORD);
	}
	else{}
	
	//CLOCK:Polarity and phase.
	if(MD == 0){SPCR &= ~((1<<CPOL) | (1<<CPHA));}
	else if (MD == 1){SPCR &= ~(1<<CPOL);SPCR |= (1<<CPHA);}
	else if (MD == 2){SPCR |= (1<<CPOL);SPCR &= ~(1<<CPHA);}
	else if (MD == 3){SPCR |= ((1<<CPOL) | (1<<CPHA));}
	else{}
	
	//CLOCK RATE: 2-128.
	// Clear clock bits first
	SPCR &= ~((1<<SPR1)|(1<<SPR0));
	SPSR &= ~(1<<SPI2X);

	if (FOSC == 2) {
		// 2 = SPI2X=1, SPR1=0, SPR0=0
		SPSR |= (1<<SPI2X);
	}
	else if (FOSC == 4) {
		// 4 = SPI2X=0, SPR1=0, SPR0=0
	}
	else if (FOSC == 8) {
		// 8 = SPI2X=1, SPR1=0, SPR0=1
		SPCR |= (1<<SPR0);
		SPSR |= (1<<SPI2X);
	}
	else if (FOSC == 16) {
		// 16 = SPI2X=0, SPR1=0, SPR0=1
		SPCR |= (1<<SPR0);
	}
	else if (FOSC == 32) {
		// 32 = SPI2X=1, SPR1=1, SPR0=0
		SPCR |= (1<<SPR1);
		SPSR |= (1<<SPI2X);
	}
	else if (FOSC == 64) {
		// 64 = SPI2X=0, SPR1=1, SPR0=0
		SPCR |= (1<<SPR1);
	}
	else if (FOSC == 128) {
		// 128 = SPI2X=0, SPR1=1, SPR0=1
		SPCR |= (1<<SPR1) | (1<<SPR0);
	}
	else{
		//4 doble speed as standar:
		// 4 = SPI2X=0, SPR1=0, SPR0=0
	}
	
	//ENABLE_SPI:
	SPCR |= (1<<SPE);
}

void SPI_MST_Transmit(uint8_t DTA){
	
	//Start transmission:
	SPDR = DTA;
	
	//Wait for transmission complete:
	while(!(SPSR&(1<<SPIF)));
}
uint8_t SPI_SLV_Receive(void){
	while (!(SPSR & (1<<SPIF)));
	return SPDR;
}