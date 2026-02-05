/*
 * USART.h
 *
 * Created: 2/4/2026 11:08:16 PM
 *  Author: Eliph
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>

extern char* itoa(int value, char* str, int base);

//SETUP:
void AS_USART(uint32_t BR, uint8_t DBL, uint8_t TXE, uint8_t RXE, uint8_t STB, uint8_t CS);
void S_USART();

//TRX: String.
void USART_TR(const char *DT);

//RX: Character -> Menu.
unsigned char USART_RC();
uint8_t USART_RC_INT();

//ADC_READ:
void USART_TR_ADC(uint8_t DT_ADC);
void USART_TR_S(char DT);
void USART_ADC_OUT();

void USART_SEND_MODE(uint8_t MD);
void USART_SEND_MM(uint8_t M1, uint8_t M2, uint8_t M3, uint8_t M4);

void USART_TR_U32(uint32_t v);

uint8_t USART_ASCII_TO_U8(volatile uint8_t *buf);


#endif /* USART_H_ */