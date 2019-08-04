/*
 * spi.c
 *
 *  Created on: 4 ���. 2019 �.
 *      Author: folla
 */

#include "spi.h"
#include "stm32f10x.h"

static uint32_t spi1_tx_couter = 0;
static uint32_t spi1_tx_size = 0;

static uint32_t spi1_rx_couter = 0;
static uint32_t spi1_rx_size = 0;

static uint8_t* tx_buffer_ptr = (void*)0;
static uint8_t* rx_buffer_ptr = (void*)0;

void SPI1_Init(void){
	/******************��������� SPI1*******************/

	/*������������ ����� A*/
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
	/*������������ �������������� ������� (���������� ����� ���� ��� ��������)*/
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);

	/* ��� SPI1:
	 *
	 * ----------------
	 *   MOSI  |  PA7
	 * ----------------
	 *   MISO  |  PA6
	 * ----------------
	 *   SCK   |  PA5
	 * ----------------
	 *   NSS   |  PA4
	 * ----------------
	 */

	//NSS - alternate push-pull
	CLEAR_BIT(GPIOA->CRL, GPIO_CRL_CNF4 | GPIO_CRL_MODE4);

	CLEAR_BIT(GPIOA->CRL, GPIO_CRL_CNF4_0);
	SET_BIT(GPIOA->CRL, GPIO_CRL_CNF4_1);

	SET_BIT(GPIOA->CRL, GPIO_CRL_MODE4_0);
	SET_BIT(GPIOA->CRL, GPIO_CRL_MODE4_1);

	//CLK - alternate push-pull
	CLEAR_BIT(GPIOA->CRL, GPIO_CRL_CNF5 | GPIO_CRL_MODE5);

	CLEAR_BIT(GPIOA->CRL, GPIO_CRL_CNF5_0);
	SET_BIT(GPIOA->CRL, GPIO_CRL_CNF5_1);

	SET_BIT(GPIOA->CRL, GPIO_CRL_MODE5_0);
	SET_BIT(GPIOA->CRL, GPIO_CRL_MODE5_1);

	//MISO - input floating
	CLEAR_BIT(GPIOA->CRL, GPIO_CRL_CNF6 | GPIO_CRL_MODE6);

	SET_BIT(GPIOA->CRL, GPIO_CRL_CNF6_0);
	CLEAR_BIT(GPIOA->CRL, GPIO_CRL_CNF6_1);

	//MOSI - alternate push-pull
	CLEAR_BIT(GPIOA->CRL, GPIO_CRL_CNF7 | GPIO_CRL_MODE7);

	CLEAR_BIT(GPIOA->CRL, GPIO_CRL_CNF7_0);
	SET_BIT(GPIOA->CRL, GPIO_CRL_CNF7_1);

	SET_BIT(GPIOA->CRL, GPIO_CRL_MODE7_0);
	SET_BIT(GPIOA->CRL, GPIO_CRL_MODE7_1);

	/*��������� ������������ SPI1*/
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
	/*�������� ������� �� 4*/
	CLEAR_BIT(SPI1->CR1, SPI_CR1_BR);
	SET_BIT(SPI1->CR1, SPI_CR1_BR_0);

	/*SPHA = 1, SPOL = 0*/
	SET_BIT(SPI1->CR1, SPI_CR1_CPHA);
	CLEAR_BIT(SPI1->CR1, SPI_CR1_CPOL);

	/*������ ����� = 8 ���*/
	CLEAR_BIT(SPI1->CR1, SPI_CR1_DFF);
	/*MSB*/
	CLEAR_BIT(SPI1->CR1, SPI_CR1_LSBFIRST);
	/*software slave managment*/
	SET_BIT(SPI1->CR1, SPI_CR1_SSM);
	SET_BIT(SPI1->CR1, SPI_CR1_SSI);

	/*������������� ��� ������*/
	SET_BIT(SPI1->CR1, SPI_CR1_MSTR);

	NVIC_EnableIRQ(SPI1_IRQn);

	/*��������� SPI1*/
	SET_BIT(SPI1->CR1, SPI_CR1_SPE);
}

uint8_t SPI1_Transmit(uint8_t* tx_buffer, uint32_t buffer_size){
	/*���� SPI �� ����� ���������*/
	if (!((SPI1->SR & SPI_SR_BSY)&(SPI1->SR & SPI_SR_OVR)&(SPI1->SR & SPI_SR_MODF))){
		spi1_tx_couter = 0;
		spi1_tx_size = buffer_size;
		tx_buffer_ptr = tx_buffer;
		SET_BIT(SPI1->CR2, SPI_CR2_TXEIE);
		return 0;
	}else{
		return 1;
	}
}

uint8_t SPI1_TransmitRecieve(uint8_t* tx_buffer, uint8_t* rx_buffer, uint32_t size){
	/*���� SPI �� ����� ���������*/
	if (!((SPI1->SR & SPI_SR_BSY)&(SPI1->SR & SPI_SR_OVR)&(SPI1->SR & SPI_SR_MODF))){

		spi1_tx_couter = 0;
		spi1_tx_size = size;
		tx_buffer_ptr = tx_buffer;

		spi1_rx_couter = 0;
		spi1_rx_size = size;
		rx_buffer_ptr = rx_buffer;

		SET_BIT(SPI1->CR2, SPI_CR2_TXEIE);
		SET_BIT(SPI1->CR2, SPI_CR2_RXNEIE);
		return 0;
	}else{
		return 1;
	}
}

void SPI1_Handler(void){
	/*���� Tx ����� ����*/
	if(READ_BIT(SPI1->SR,SPI_SR_TXE)){

		if (spi1_tx_couter < spi1_tx_size){

			*(uint8_t *)&(SPI1->DR) = tx_buffer_ptr[spi1_tx_couter];

			spi1_tx_couter++;

		}else{
			CLEAR_BIT(SPI1->CR2, SPI_CR2_TXEIE);
		}
	}
		/*���� � Rx ����� ���-�� ������*/
		if(READ_BIT(SPI1->SR,SPI_SR_RXNE)){
			if (spi1_rx_couter < spi1_rx_size){

				rx_buffer_ptr[spi1_rx_couter] = (uint8_t)SPI1->DR;

				spi1_rx_couter++;
			}else{
				CLEAR_BIT(SPI1->CR2, SPI_CR2_RXNEIE);
			}
		}
}
