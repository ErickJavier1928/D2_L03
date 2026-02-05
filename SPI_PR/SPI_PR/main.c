/*
 * SPI_PR.c
 *
 * Created: 1/29/2026 12:10:06 PM
 * Author : Eliph
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "SPIC/SPIC.h"
#include "ADC/ADC.h"
#include "USART/USART.h"

//GLOBAL: ADC.
uint8_t ADC_VALUES[8];
uint8_t ADC_ACT[8];
uint8_t ADC_ACT_CN = 0;
uint8_t ADC_CN = 0;
unsigned char RCD_DATA;

//SETUP:
void SETUP(void){
	cli();
	//TIMERS: 16 prescaler.
	CLKPR = (1 << CLKPCE);
	CLKPR = (1 << CLKPS2);
	
	//OUTPUT LEDs:
	DDRD = 0xFF;
	
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
ISR(ADC_vect){
	//STORE:
	ADC_VALUES[ADC_ACT[ADC_CN]] = ADCH;
	
	//INC:
	ADC_CN++;
	if (ADC_CN == ADC_ACT_CN){
		ADC_CN = 0;
	}
	
	//NEW:
	ADMUX = (ADMUX & 0xF0) | (ADC_ACT[ADC_CN]& 0x0F);
	
	//START:
	ADCSRA |= (1<<ADSC);
}
ISR(SPI_STC_vect){
	//READ:
	uint8_t SPI_VALUE = SPDR;
	
	//CONDITION:
	if(SPI_VALUE == 'S'){
		//ADC: PC0.
		SPDR = ADC_VALUES[0];
	}
	else if(SPI_VALUE == 'D'){
		//ADC: PC1.
		SPDR = ADC_VALUES[1];
	}
	else{}
}

int main(void)
{
	//SETUP:
	SETUP();
	
	//ADC:
	ADC_PC05_67(1,0,2,1,0,1,8,0);
	
	//ADC selection: PC0 and PC1;
	uint8_t ADC_SL[2] = {0,1};
	ALT_ADC(ADC_SL, 2);
	
	//USART:
	AS_USART(9600,1,1,1,1,8);
	
    /* Replace with your application code */
    while (1) 
    {
		USART_TR("_______________________________");
		USART_TR("\r\n");
		USART_TR("ADC -> PC0:");
		USART_TR_ADC(ADC_VALUES[0]);
		USART_ADC_OUT();
		USART_TR("\r\n");
		
		
		USART_TR("\r\n");
		USART_TR("ADC -> PC1:");
		USART_TR_ADC(ADC_VALUES[1]);
		USART_ADC_OUT();
		USART_TR("\r\n");
		
		USART_TR("_______________________________");
    }
}

