/*
 * clock_setup.c
 *
 *  Created on: 3 авг. 2019 г.
 *      Author: folla
 */

#include "clock_setup.h"
#include "stm32f10x.h"

void ClockSetup(void){
	/* Disable all interrupts and clear pending bits  */
	  RCC->CIR = 0x009F0000;
	  /*включить HSE*/
	  SET_BIT(RCC->CR, RCC_CR_HSEON);
	  /*ждём пока не включится HSE*/
	  while(!(RCC->CR & RCC_CR_HSERDY));

	  /*включить CSS*/
	  SET_BIT(RCC->CR, RCC_CR_CSSON);

	  /*выбираем HSE как SYSCLK */
	  SET_BIT(RCC->CFGR, RCC_CFGR_SW_HSE);

	  //принудительно отключаем PLL
	  CLEAR_BIT(RCC->CR, RCC_CR_PLLON);

	  //ставим источник PLL как сигнал с HSE, делённый предделителями PLL
	  SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC_HSE);

	  //ставим множитель PLL
	  SET_BIT(RCC->CFGR, RCC_CFGR_PLLMULL9);

	  /*Настройка FLASH*/
	  CLEAR_REG(FLASH->ACR);
	  SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE);
	  SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_2);


	  /*
	   * Настройка тактирования шин
	   */
	  CLEAR_BIT(RCC->CFGR, RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_HPRE);

	  //частота АРВ1 в 36Мгц по даташиту
	  SET_BIT(RCC->CFGR, RCC_CFGR_PPRE1_DIV2);
	  SET_BIT(RCC->CFGR, RCC_CFGR_PPRE2_DIV2);
	  /* *  */

	  //включаем PLL
	  SET_BIT(RCC->CR, RCC_CR_PLLON);

	  /*Ждём пока PLL не включится*/
	  while(!(RCC->CR & RCC_CR_PLLRDY));

	  //выбираем PLL как источник SYSCLK
	  CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW);
	  SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);

	  //ждём пока он не выберется
	  while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
}
