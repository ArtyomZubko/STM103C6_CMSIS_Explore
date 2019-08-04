/*
 * stm32f10x_it.h
 *
 *  Created on: 3 рту. 2019 у.
 *      Author: folla
 */

#ifndef INC_STM32F10X_IT_H_
#define INC_STM32F10X_IT_H_

#include <stdint.h>
extern uint32_t spi_tx_counter;
extern uint8_t spi_tx_buffer[5];
uint8_t entry ;

void NMI_Handler(void);
void SysTick_Handler(void);
void SPI1_IRQHandler(void);

#endif /* INC_STM32F10X_IT_H_ */
