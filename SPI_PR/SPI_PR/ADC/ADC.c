/*
 * ADC.c
 *
 * Created: 1/29/2026 12:15:09 PM
 *  Author: Eliph
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "ADC.h"

void ADC_PC05_67(uint8_t EN, uint8_t FISL, uint8_t VREF, uint8_t JR, uint8_t AT, uint8_t IE, uint8_t PRS, uint8_t SR){
	
	//ADMUX:
	if (FISL == 0) {
		ADMUX &= 0xF0; // Clear MUX bits
	}
	else if (FISL == 1) {
		ADMUX &= 0xF0;
		ADMUX |= (1 << MUX0);
	}
	else if (FISL == 2) {
		ADMUX &= 0xF0;
		ADMUX |= (1 << MUX1);
	}
	else if (FISL == 3) {
		ADMUX &= 0xF0;
		ADMUX |= (1 << MUX0) | (1 << MUX1);
	}
	else if (FISL == 4) {
		ADMUX &= 0xF0;
		ADMUX |= (1 << MUX2);
	}
	else if (FISL == 5) {
		ADMUX &= 0xF0;
		ADMUX |= (1 << MUX0) | (1 << MUX2);
	}
	else if (FISL == 6) {
		ADMUX &= 0xF0;
		ADMUX |= (1 << MUX1) | (1 << MUX2);
	}
	else if (FISL == 7) {
		ADMUX &= 0xF0;
		ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << MUX2);
	}

	//VREF: 1.1V internal, default.
	//CLEAR:
	ADMUX &= ~(1 << REFS1);
	ADMUX &= ~(1 << REFS0);
	
	if (VREF == 1){
		//Internal 1.1V:
		ADMUX |= (1<<REFS0) | (1<<REFS1);
	}
	
	else if (VREF == 2){
		//AVcc with external capacitor at AREF pin.
		ADMUX |= (1<<REFS0);
	}
	else if (VREF == 0){
		//Off:
	}
	
	else {
		//Internal 1.1V:
		ADMUX |= (1<<REFS0) | (1<<REFS1);
	}
	
	//ADLAR: JD default.
	ADMUX &= ~(1 << ADLAR);

	if (JR == 1){
		// Left adjust result
		ADMUX |= (1 << ADLAR);
	}
	
	//ADCSRA: Enable.
	if (EN == 1){
		ADCSRA |= (1<<ADEN);
	}
	else {}
	
	if (AT == 1){
		ADCSRA |= (1 << ADATE);  // Auto-trigger mode
	}
	else {
		ADCSRA |= (1 << ADSC);   // Manual mode, start now
	}
	
	//Interrupt enable:
	if (IE == 1){
		ADCSRA |= (1<<ADIE);
	}
	else {}
	
	//Prescaler: 2 default.
	ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
	
	if(PRS == 2){
		ADCSRA |= (1<<ADPS0);
	}
	else if(PRS == 4){
		ADCSRA |= (1<<ADPS1);
	}
	else if(PRS == 8){
		ADCSRA |= (1<<ADPS0)| (1<<ADPS1);
	}
	else if(PRS == 16){
		ADCSRA |= (1<<ADPS2);
	}
	else if(PRS == 32){
		ADCSRA |= (1<<ADPS0)|(1<<ADPS2);
	}
	else if(PRS == 64){
		ADCSRA |= (1<<ADPS1)|(1<<ADPS2);
	}
	else if(PRS == 128){
		ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	}
	else{
	}
	
	//ADCSRB: Source.
	// Clear ADTS2:0 first
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

	if (SR == 0) {
		// Free Running mode
	}
	else if (SR == 1) {
		// Analog Comparator
		ADCSRB |= (1 << ADTS0);
	}
	else if (SR == 2) {
		// External Interrupt Request 0
		ADCSRB |= (1 << ADTS1);
	}
	else if (SR == 3) {
		// Timer/Counter0 Compare Match A
		ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
	}
	else if (SR == 4) {
		// Timer/Counter0 Overflow
		ADCSRB |= (1 << ADTS2);
	}
	else if (SR == 5) {
		// Timer/Counter1 Compare Match B
		ADCSRB |= (1 << ADTS2) | (1 << ADTS0);
	}
	else if (SR == 6) {
		// Timer/Counter1 Overflow
		ADCSRB |= (1 << ADTS2) | (1 << ADTS1);
	}
	else if (SR == 7) {
		// Timer/Counter1 Capture Event
		ADCSRB |= (1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0);
	}
	else {
		// Default to Free Running
		ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
	}

	//ADCH y ADCL:
	
	
}
void ALT_ADC(uint8_t *ACT, uint8_t COUNT){
	
	//STORE:
	for (uint8_t i = 0; i < COUNT; i++){
		ADC_ACT[i] = ACT[i] & 0x07;
	}
	
	ADC_ACT_CN = COUNT;
	
}

//ADC mapping:

//Timer 0 and 2: 0-255 (0->180).
uint8_t ADC_DC_T02(uint8_t EADC){
	uint16_t TP = ((uint16_t)EADC * 37);
	uint8_t PDC = 8 + TP/255;
	return PDC;
}

//OP2: 0 -> 60
uint8_t ADC_DC_T02_60(uint8_t EADC){
	uint16_t TP = ((uint16_t)EADC * 18);
	uint8_t PDC = 8 + TP/255;
	return PDC;
}

//OP3: 0 -> 45
uint8_t ADC_DC_T02_45(uint8_t EADC){
	uint16_t TP = ((uint16_t)EADC * 15);
	uint8_t PDC = 8 + TP/255;
	return PDC;
}

//Timer 1: 0-1024 (0->180).
uint16_t ADC_DC_T1(uint8_t EADC){
	uint32_t TP = ((uint32_t)EADC * 1900);
	uint16_t PDC = 500 + ((TP)/255);
	return PDC;
}