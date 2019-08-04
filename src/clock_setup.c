/*
 * clock_setup.c
 *
 *  Created on: 3 ���. 2019 �.
 *      Author: folla
 */

#include "clock_setup.h"
#include "stm32f10x.h"

void ClockSetup(void){
	/* Disable all interrupts and clear pending bits  */
	  RCC->CIR = 0x009F0000;
	  /*�������� HSE*/
	  SET_BIT(RCC->CR, RCC_CR_HSEON);
	  /*��� ���� �� ��������� HSE*/
	  while(!(RCC->CR & RCC_CR_HSERDY));

	  /*�������� CSS*/
	  SET_BIT(RCC->CR, RCC_CR_CSSON);

	  /*�������� HSE ��� SYSCLK */
	  SET_BIT(RCC->CFGR, RCC_CFGR_SW_HSE);

	  //������������� ��������� PLL
	  CLEAR_BIT(RCC->CR, RCC_CR_PLLON);

	  //������ �������� PLL ��� ������ � HSE, ������� �������������� PLL
	  SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC_HSE);

	  //������ ��������� PLL
	  SET_BIT(RCC->CFGR, RCC_CFGR_PLLMULL9);

	  /*��������� FLASH*/
	  CLEAR_REG(FLASH->ACR);
	  SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE);
	  SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_2);


	  /*
	   * ��������� ������������ ���
	   */
	  CLEAR_BIT(RCC->CFGR, RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_HPRE);

	  //������� ���1 � 36��� �� ��������
	  SET_BIT(RCC->CFGR, RCC_CFGR_PPRE1_DIV2);
	  SET_BIT(RCC->CFGR, RCC_CFGR_PPRE2_DIV2);
	  /* *  */

	  //�������� PLL
	  SET_BIT(RCC->CR, RCC_CR_PLLON);

	  /*��� ���� PLL �� ���������*/
	  while(!(RCC->CR & RCC_CR_PLLRDY));

	  //�������� PLL ��� �������� SYSCLK
	  CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW);
	  SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);

	  //��� ���� �� �� ���������
	  while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
}
