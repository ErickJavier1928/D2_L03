/*
 * USART.c
 *
 * Created: 1/29/2026 8:43:54 PM
 *  Author: Eliph
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "USART.h"

uint8_t ASCII_CN = 0x00;
uint8_t CR_00 = 0xFF;
uint8_t	CR_01 = 0xFF;
uint8_t CR_02 = 0xFF;

//GLOBAL:

//MODE: Asynchronous -> BR(Baud_rate), DBL(Doble_speed).
void AS_USART(uint32_t BR, uint8_t DBL, uint8_t TXE, uint8_t RXE, uint8_t STB, uint8_t CS){
	//CLEAR:
	UCSR0A &= ~(1 << U2X0);  // Clear double speed
	UCSR0B = 0;
	UCSR0C = 0;
	
	uint32_t TMP = 0x0000;
	
	if(DBL == 1){
		TMP = ((1000000/(8*BR)) - 1);

	}
	else if (DBL == 0){
		TMP = ((1000000/(16*BR)) - 1);
	}
	
	//BR:
	UBRR0L = ((uint8_t)TMP);
	UBRR0H = (TMP >> 8);
	
	//Doble_speed:
	if (DBL == 1){
		UCSR0A |= (1<< U2X0);
	}
	else {}
	
	//Transmitter_Receiver:
	if(TXE == 1){
		UCSR0B |= (1 << TXEN0);
	}
	else {}
	
	if(RXE == 1){
		UCSR0B |= (1 << RXEN0);
	}
	else{}
	
	//STOP_BIT: 1 or 2.
	if (STB == 1){
		UCSR0C &= ~(1 << USBS0);
	}
	else if(STB == 2){
		UCSR0C |= (1<<USBS0);
	}
	else {
		UCSR0C &= ~(1 << USBS0);
	}
	
	//CLEAR:
	UCSR0B &= ~(1 << UCSZ02);
	UCSR0C &= ~((1 << UCSZ00) | (1 << UCSZ01));
	
	//Character size: 8-bits default.
	if(CS == 5){}
	else if (CS == 6){
		UCSR0C |= (1<<UCSZ00);
	}
	else if (CS == 7){
		UCSR0C |= (1<<UCSZ01);
		
	}
	else if (CS == 8){
		UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	}
	
	else if (CS == 9){
		UCSR0B |= (1<<UCSZ02);
		UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	}
	else {
		UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	}
	
	//RX_ISR: ENABLE.
	UCSR0B |= (1 << RXCIE0);

	
	
}

//MODE: Synchronous.
void S_USART(){}

//TRX: String.
void USART_TR(const char *DT){
	while(*DT != '\0'){
		
		//EP:
		while(!(UCSR0A & (1 << UDRE0)));
		
		//LOAD_TO_TRANSF:
		UDR0 = *DT;
		
		//INC: DIR.
		DT++;
	}
}

//RX: Character -> Menu.
unsigned char USART_RC(){
	//DATA_RECEIVED:
	while(!(UCSR0A & (1 << RXC0)));
	
	//RETURN:
	return UDR0;
}

//RX: Int
uint8_t USART_RC_INT(){
	//DATA_RECEIVED:
	while(!(UCSR0A & (1 << RXC0)));
	
	//RETURN:
	return UDR0;
}

//ADC_READ:
void USART_TR_ADC(uint8_t DT_ADC){
	//DIV:
	if (DT_ADC >= 100)	{
		CR_02 = (DT_ADC/100) + '0';	//ASCII: 0 -> 0x30/48.
		DT_ADC %= 100;
		
		CR_01 = (DT_ADC/10) + '0';
		DT_ADC %= 10;
		
		CR_00 = (DT_ADC) + '0';
		
		//COUNTER:
		ASCII_CN = 0x03;
	}
	else if (DT_ADC >= 10){
		CR_01 = (DT_ADC/10) + '0';
		DT_ADC %= 10;
		
		CR_00 = (DT_ADC) + '0';
		
		//COUNTER:
		ASCII_CN = 0x02;
	}
	else if (DT_ADC >= 0){
		CR_00 = (DT_ADC) + '0';
		
		//COUNTER:
		ASCII_CN = 0x01;
	}
	else {
		//COUNTER: Error to read ADC.
		ASCII_CN = 0x00;
	}
}

//OUT: ADC.
void USART_TR_S(char DT){
	//EP:
	while(!(UCSR0A & (1 << UDRE0)));
	
	//LOAD_TO_TRANSF:
	UDR0 = DT;
	
	//WAIT_TRANSF:
	while (!(UCSR0A & (1 << TXC0)));
	UCSR0A |= (1 << TXC0);
}
void USART_ADC_OUT(){
	if (ASCII_CN  == 0x03){
		USART_TR_S((char)CR_02);
		USART_TR_S((char)CR_01);
		USART_TR_S((char)CR_00);
	}
	else if (ASCII_CN == 0x02){
		USART_TR_S((char)CR_01);
		USART_TR_S((char)CR_00);
	}
	else if (ASCII_CN  == 0x01){
		USART_TR_S((char)CR_00);
	}
	else {
		//ERROR:
		USART_TR("ERROR");
	}
}

//
void USART_TR_U32(uint32_t v)
{
	char buf[11];   // max 10 digits + null
	uint8_t i = 10;
	buf[i] = '\0';

	if(v == 0){
		USART_TR("0");
		return;
	}

	while(v > 0 && i){
		i--;
		buf[i] = (v % 10) + '0';
		v /= 10;
	}

	USART_TR(&buf[i]);
}
