/*
 * stm32f10x_it.c
 *
 *  Created on: 3 рту. 2019 у.
 *      Author: folla
 */

#include "stm32f10x_it.h"
#include "stm32f10x.h"
#include "spi.h"

 uint32_t spi_tx_counter = 0;
uint8_t spi_tx_buffer[5] = {0};
uint8_t entry = 0;
void NMI_Handler(void){
	if(READ_BIT(RCC->CIR,RCC_CIR_CSSF)){
		SysTick_Config(8000000/1000);
		SET_BIT(RCC->CIR,RCC_CIR_CSSC);
	}
}

void SysTick_Handler(void) {
	//ticks_delay++;
}

void SPI1_IRQHandler(void){
	SPI1_Handler();
}
