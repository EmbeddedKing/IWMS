/******************** (C) COPYRIGHT 2010 Embest Info&Tech Co.,LTD. ************
* File Name          : retarget.c
* Author             : Wuhan R&D Center, Embest
* Date First Issued  : 01/18/2010
* Description        : Redefine fputc function.
*******************************************************************************
*******************************************************************************
* History:
* 01/18/2010		 : V1.0		   initial version
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "LPC11xx.h"
#include "uart.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/  

/**
  * @brief  Redefine fputc function.
  * @param  ...
  * @retval ...
  */
int fputc(int ch, FILE *f)
{
  /* Loop until the end of transmission */
  while (!(LPC_UART->LSR & LSR_THRE))
  {	  
  }
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */  
  LPC_UART->THR = ch;

  return ch;
}


/**
  * @}
  */ 

/**
  * @}
  */ 

/************* (C) COPYRIGHT 2010 Wuhan R&D Center, Embest *****END OF FILE****/
