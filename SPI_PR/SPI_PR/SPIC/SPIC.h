/*
 * SPIC.h
 *
 * Created: 1/29/2026 12:11:02 PM
 *  Author: Eliph
 */ 


#ifndef SPIC_H_
#define SPIC_H_


#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

//TYPE: MASTER or SLAVE.
typedef enum
{SPI_MASTER,SPI_SLAVE} SPI_Role;

//READING FASE: Based on polarity(AH or AL) and rising/falling edge.
typedef enum
{SPI_MODE0,SPI_MODE1,SPI_MODE2,SPI_MODE3} SPI_Mode;

//DATA ORDER:
typedef enum
{SPI_LSB, SPI_MSB} SPI_Data_Order;

//DOUBLE:
typedef enum
{SPI_NDB, SPI_DB} SPI_Speed;

void SPI_SETUP(SPI_Role RL, SPI_Mode MD,SPI_Data_Order DO, uint8_t FOSC);
void SPI_MST_Transmit(uint8_t DTA);
uint8_t SPI_SLV_Receive(void);



#endif /* SPIC_H_ */