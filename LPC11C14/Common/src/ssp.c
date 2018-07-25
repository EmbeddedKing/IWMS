/****************************************************************************
 *   $Id:: ssp.c 3635 2010-06-02 00:31:46Z usb00423                         $
 *   Project: NXP LPC11xx SSP example
 *
 *   Description:
 *     This file contains SSP code example which include SSP 
 *     initialization, SSP interrupt handler, and APIs for SSP
 *     reading.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "LPC11xx.h"			/* LPC11xx Peripheral Registers */
#include "gpio.h"
#include "ssp.h"
#include "uart.h"
#include <stdio.h>
/* statistics of all the interrupts */
volatile uint32_t interruptRxStat0 = 0;
volatile uint32_t interruptOverRunStat0 = 0;
volatile uint32_t interruptRxTimeoutStat0 = 0;

volatile uint32_t interruptRxStat1 = 0;
volatile uint32_t interruptOverRunStat1 = 0;
volatile uint32_t interruptRxTimeoutStat1 = 0;
extern unsigned char command[50];
extern  unsigned char global; 
extern   unsigned char count1;
extern   unsigned char count2;
volatile unsigned char flag = 0;
unsigned char flag1 = 0;
extern unsigned char a ;
//volatile uint8_t src_addr[SSP_BUFSIZE]; 
//volatile uint8_t dest_addr[SSP_BUFSIZE];
 /*****************************************************************************
** Function name:		SSP_Send
**
** Descriptions:		Send a block of data to the SSP port, the 
**						first parameter is the buffer pointer, the 2nd 
**						parameter is the block length.
**
** parameters:			port #, buffer pointer, and the block length
** Returned value:		None
** 
*****************************************************************************/

 unsigned short SPI0_masterRS( unsigned char  portNum, unsigned char  buf )
{ 
	if ( portNum == 0 )
	{
	  GPIOSetDir( PORT2, 7, 1 );
	  GPIOSetValue( PORT2, 7, 1);
	  GPIOSetValue( PORT2, 7, 0 ); 
	  while (( !(LPC_SSP0->SR & SSPSR_TNF)||(LPC_SSP0->SR & SSPSR_BSY)) != 0 );
	  LPC_SSP0->DR = buf;
	  while ( LPC_SSP0->SR & SSPSR_BSY );
	  /* Wait until the Busy bit is cleared */
      while((LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE);
   	  GPIOSetValue( PORT2, 7, 1);
    }
  return	  LPC_SSP0->DR;
}
unsigned short SPI0_slaveRS( unsigned char  portNum, unsigned char const  buf )
{ 
	if ( portNum == 0 )
	{
	  while (( !(LPC_SSP0->SR & SSPSR_TNF)||(LPC_SSP0->SR & SSPSR_BSY)) != 0 );
	  LPC_SSP0->DR = buf;
	  while ( LPC_SSP0->SR & SSPSR_BSY );
	  /* Wait until the Busy bit is cleared */
      while((LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE);
    }
  return	  LPC_SSP0->DR;
}
/*****************************************************************************
** Function name:		SSP0_IRQHandler
**
** Descriptions:		SSP port is used for SPI communication.
**						SSP interrupt handler
**						The algorithm is, if RXFIFO is at least half full, 
**						start receive until it's empty; if TXFIFO is at least
**						half empty, start transmit until it's full.
**						This will maximize the use of both FIFOs and performance.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/

void SSP0_IRQHandler(void) 
{
  uint32_t regValue;
  static unsigned char y = 0;
  unsigned short temp = 0;
  
   regValue = LPC_SSP0->MIS;	  	

  if ( regValue & SSPMIS_RORMIS )	/* Receive overrun interrupt */
  {
	interruptOverRunStat0++;
	LPC_SSP0->ICR = SSPICR_RORIC;	/* clear interrupt */
  }
  if ( regValue & SSPMIS_RTMIS )	/* Receive timeout interrupt */
  {
/*	LPC_UART->THR = LPC_SSP0->DR;
	while ( !(LPC_UART->LSR & LSR_THRE) );	 */
	
	if(flag == 1)
	 {	
	 	y++;
		temp	= 	LPC_SSP0->DR;
	 	command[y] =  temp;
		if(y == command[2]+2)
		{				
			flag1 = 0;
			y=0;
		 	global =1;
		 	flag =0;
		if(a == 1)	{NVIC_DisableIRQ(SSP0_IRQn);};
		}  
	 }
	if(flag1 == 0)
	{
	 	temp	= 	LPC_SSP0->DR;
	 	if(0xAA == temp)
	 	{
	  		flag = 1;
			flag1 = 1;
	  		y=0;
	  		command[y] = temp;
	  	}
	}
   	 LPC_SSP0->ICR = SSPICR_RTIC | SSPICR_RORIC;
  }

  /* please be aware that, in main and ISR, CurrentRxIndex and CurrentTxIndex
  are shared as global variables. It may create some race condition that main
  and ISR manipulate these variables at the same time. SSPSR_BSY checking (polling)
  in both main and ISR could prevent this kind of race condition */
  if ( regValue & SSPMIS_RXMIS )	/* Rx at least half full */
  {
	interruptRxStat0++;		/* receive until it's empty  */
  }

  return;
}	   

/*****************************************************************************
** Function name:		SSP_IOConfig
**
** Descriptions:		SSP port initialization routine
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SSP_IOConfig( uint8_t portNum )
{
  if ( portNum == 0 )
  {
	LPC_SYSCON->PRESETCTRL |= (0x1<<0);
	LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<11);
	LPC_SYSCON->SSP0CLKDIV = 0x02;				/* Divided by 2 */
	LPC_IOCON->PIO0_8           &= ~0x07;		/* SSP I/O config */
	LPC_IOCON->PIO0_8           |= 0x01;		/* SSP MISO */
	LPC_IOCON->PIO0_9           &= ~0x07;	
	LPC_IOCON->PIO0_9           |= 0x01;		/* SSP MOSI */
	LPC_IOCON->SCKLOC = 0x02;
	LPC_IOCON->PIO0_6 = 0x02;		/* P0.6 function 2 is SSP clock, need to */
	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);									
#ifdef   SSP_SLAVE 
	LPC_IOCON->PIO0_2 &= ~0x07;	
	LPC_IOCON->PIO0_2 |= 0x01;		/* SSP SSEL */
#else
   	LPC_IOCON->PIO2_7 &= ~0x07;		/* SSP SSEL2 is a GPIO pin, for RFID */
	/* port2, bit 5 is set to GPIO output and high */
	GPIOSetDir( PORT2, 7, 1 );
	GPIOSetValue( PORT2, 7, 1 );
#endif
  }
else		/* port number 1 */
  {
	LPC_SYSCON->PRESETCTRL |= (0x1<<2);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<18);
	LPC_SYSCON->SSP1CLKDIV = 0x02;	/* Divided by 2 */
	LPC_IOCON->PIO2_2 &= ~0x07;		/*  SSP I/O config */
	LPC_IOCON->PIO2_2 |= 0x02;		/* SSP MISO */
	LPC_IOCON->PIO2_3 &= ~0x07;	
	LPC_IOCON->PIO2_3 |= 0x02;		/* SSP MOSI */
	LPC_IOCON->PIO2_1 &= ~0x07;
	LPC_IOCON->PIO2_1 |= 0x02;		/* SSP CLK */
 
#if USE_CS
	LPC_IOCON->PIO2_0 &= ~0x07;	
	LPC_IOCON->PIO2_0 |= 0x02;		/* SSP SSEL */
#else
	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

	LPC_IOCON->PIO2_0 &= ~0x07;		/* SSP SSEL is a GPIO pin */
	/* port2, bit 0 is set to GPIO output and high */
	GPIOSetDir( PORT2, 0, 1 );
	GPIOSetValue( PORT2, 0, 1 );
#endif
  }
  return;		
}

/*****************************************************************************
** Function name:		SPI_IOConfig
**
** Descriptions:		SPI port initialization routine
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SPI_IOConfig(uint8_t portNum)
{
  if(portNum == 0)
  {
	LPC_SYSCON->PRESETCTRL    |= (0x1<<0);
	LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<11);
	LPC_SYSCON->SSP0CLKDIV     = 0x02;			/* Divided by 2 */
	LPC_IOCON->PIO0_8         &= ~0x07;			/* SSP I/O config */
	LPC_IOCON->PIO0_8         |= 0x01;			/* SSP MISO */
	LPC_IOCON->PIO0_9         &= ~0x07;	
	LPC_IOCON->PIO0_9         |= 0x01;			/* SSP MOSI */

	LPC_IOCON->SCKLOC = 0x02;
	LPC_IOCON->PIO0_6 = 0x02;		/* P0.6 function 2 is SSP clock, need to 
									combined with IOCONSCKLOC register setting */  

	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

	LPC_IOCON->PIO2_4 &= ~0x07;		/* SSP SSEL0 is a GPIO pin, for OLED */
	/* port2, bit 4 is set to GPIO output and high */
	GPIOSetDir( PORT2, 4, 1 );
	GPIOSetValue( PORT2, 4, 1 );

	LPC_IOCON->PIO2_6 &= ~0x07;		/* SSP SSEL1 is a GPIO pin, for FLASH */
	/* port2, bit 6 is set to GPIO output and high */
	GPIOSetDir( PORT2, 6, 1 );
	GPIOSetValue( PORT2, 6, 1 );

	LPC_IOCON->PIO2_7 &= ~0x07;		/* SSP SSEL2 is a GPIO pin, for RFID */
	/* port2, bit 5 is set to GPIO output and high */
	GPIOSetDir( PORT2, 7, 1 );
	GPIOSetValue( PORT2, 7, 1 );
  }

  else		/* port number 1 */
  {
	LPC_SYSCON->PRESETCTRL    |= (0x1<<2);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<18);
	LPC_SYSCON->SSP1CLKDIV     = 0x02;		/* Divided by 2 */
	LPC_IOCON->PIO2_2         &= ~0x07;		/*  SSP I/O config */
	LPC_IOCON->PIO2_2         |= 0x02;		/* SSP MISO */
	LPC_IOCON->PIO2_3         &= ~0x07;	
	LPC_IOCON->PIO2_3         |= 0x02;		/* SSP MOSI */
	LPC_IOCON->PIO2_1         &= ~0x07;
	LPC_IOCON->PIO2_1         |= 0x02;		/* SSP CLK */
 
	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

	LPC_IOCON->PIO2_0 &= ~0x07;		/* SSP SSEL is a GPIO pin */
	/* port2, bit 0 is set to GPIO output and high */
	GPIOSetDir( PORT2, 0, 1 );
	GPIOSetValue( PORT2, 0, 1 );
  }		
}

/*****************************************************************************
** Function name:		SSP_Init
**
** Descriptions:		SSP port initialization routine
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SSP_Init( uint8_t portNum )
{
  uint8_t i, Dummy=Dummy;

  if ( portNum == 0 )
  {
	/* Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 15 */
	LPC_SSP0->CR0 = 0x0707;

	/* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */
//	LPC_SSP0->CPSR = 0x2;
	LPC_SSP0->CPSR = 8;

	for ( i = 0; i < SSP_FIFOSIZE; i++ )
	{
	  Dummy = LPC_SSP0->DR;		/* clear the RxFIFO */
	}

	/* Enable the SSP Interrupt */
//	NVIC_EnableIRQ(SSP0_IRQn);
	

#if SSP_SLAVE
	/* Slave mode */
	 NVIC_EnableIRQ(SSP0_IRQn);
	if ( LPC_SSP0->CR1 & SSPCR1_SSE )
	{
	  /* The slave bit can't be set until SSE bit is zero. */
	  LPC_SSP0->CR1 &= ~SSPCR1_SSE;
	}
	LPC_SSP0->CR1 = SSPCR1_MS;		/* Enable slave bit first */
	LPC_SSP0->CR1 &= ~(1<<3);		/*enable slave output*/
	LPC_SSP0->CR1 |= SSPCR1_SSE;	/* Enable SSP */
#else
	/* Master mode */
	LPC_SSP0->CR1 = SSPCR1_SSE;
#endif
	/* Set SSPINMS registers to enable interrupts */
	/* enable all error related interrupts */
	LPC_SSP0->IMSC =SSPIMSC_RORIM | SSPIMSC_RTIM |SSPIMSC_RXIM  ;
  }
  return;
}




/******************************************************************************
**                            End Of File
******************************************************************************/

