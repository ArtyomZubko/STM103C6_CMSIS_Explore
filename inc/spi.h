/*
 * spi.h
 *
 *  Created on: 4 рту. 2019 у.
 *      Author: folla
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

void SPI1_Init(void);
void SPI1_Handler(void);
uint8_t SPI1_Transmit(uint8_t* tx_buffer, uint32_t buffer_size);
uint8_t SPI1_TransmitRecieve(uint8_t* tx_buffer, uint8_t* rx_buffer, uint32_t size);

#endif /* SPI_H_ */

