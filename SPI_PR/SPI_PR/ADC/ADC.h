/*
 * ADC.h
 *
 * Created: 1/29/2026 12:14:55 PM
 *  Author: Eliph
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

extern uint8_t ADC_ACT[8];
extern uint8_t ADC_ACT_CN;

void ADC_PC05_67(uint8_t EN, uint8_t FISL, uint8_t VREF, uint8_t JR, uint8_t AT, uint8_t IE, uint8_t PRS, uint8_t SR);
void ALT_ADC(uint8_t *ACT, uint8_t COUNT);

uint8_t ADC_DC_T02(uint8_t EADC);
uint8_t ADC_DC_T02_60(uint8_t EADC);
uint8_t ADC_DC_T02_45(uint8_t EADC);
uint16_t ADC_DC_T1(uint8_t EADC);


#endif /* ADC_H_ */