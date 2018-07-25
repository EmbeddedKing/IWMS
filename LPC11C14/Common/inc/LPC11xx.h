/******************************************************************************
 * @file:    LPC11xx.h
 * @purpose: CMSIS Cortex-M0 Core Peripheral Access Layer Header File for 
 *           NXP LPC11xx Device Series 
 * @version: V1.0
 * @date:    21. Nov. 2008
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2009 NXP Semiconductors. All rights reserved.
 *
 * NXP Semiconductors is supplying this software for use with Cortex-M3 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#ifndef __LPC11xx_H__
#define __LPC11xx_H__

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M0 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M0 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M0 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M0 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M0 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M0 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M0 System Tick Interrupt               */

/******  LPC11xx Specific Interrupt Numbers *******************************************************/
  WAKEUP0_IRQn                  = 0,        /*!< All I/O pins can be used as wakeup source.       */
  WAKEUP1_IRQn                  = 1,        /*!< There are 40 pins in total for LPC11xx           */
  WAKEUP2_IRQn                  = 2,
  WAKEUP3_IRQn                  = 3,
  WAKEUP4_IRQn                  = 4,   
  WAKEUP5_IRQn                  = 5,        
  WAKEUP6_IRQn                  = 6,        
  WAKEUP7_IRQn                  = 7,        
  WAKEUP8_IRQn                  = 8,        
  WAKEUP9_IRQn                  = 9,        
  WAKEUP10_IRQn                 = 10,       
  WAKEUP11_IRQn                 = 11,       
  WAKEUP12_IRQn                 = 12,       
  CAN_IRQn                      = 13,       /*!< CAN Interrupt                                    */
  SSP1_IRQn                     = 14,       /*!< SSP1 Interrupt                                   */
  I2C_IRQn                      = 15,       /*!< I2C Interrupt                                    */
  TIMER_16_0_IRQn               = 16,       /*!< 16-bit Timer0 Interrupt                          */
  TIMER_16_1_IRQn               = 17,       /*!< 16-bit Timer1 Interrupt                          */
  TIMER_32_0_IRQn               = 18,       /*!< 32-bit Timer0 Interrupt                          */
  TIMER_32_1_IRQn               = 19,       /*!< 32-bit Timer1 Interrupt                          */
  SSP0_IRQn                     = 20,       /*!< SSP0 Interrupt                                   */
  UART_IRQn                     = 21,       /*!< UART Interrupt                                   */
  USB_IRQn                      = 22,       /*!< USB Regular Interrupt                            */
  USB_FIQn                      = 23,       /*!< USB Fast Interrupt                               */
  ADC_IRQn                      = 24,       /*!< A/D Converter Interrupt                          */
  WDT_IRQn                      = 25,       /*!< Watchdog timer Interrupt                         */  
  BOD_IRQn                      = 26,       /*!< Brown Out Detect(BOD) Interrupt                  */
  RESERVED_IRQn                 = 27,       /*!< Reserved Interrupt                               */
  EINT3_IRQn                    = 28,       /*!< External Interrupt 3 Interrupt                   */
  EINT2_IRQn                    = 29,       /*!< External Interrupt 2 Interrupt                   */
  EINT1_IRQn                    = 30,       /*!< External Interrupt 1 Interrupt                   */
  EINT0_IRQn                    = 31,       /*!< External Interrupt 0 Interrupt                   */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          5         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */


#include "core_cm0.h"                    /* Cortex-M0 processor and core peripherals           */
#include "system_LPC11xx.h"                 /* System Header                                      */



/**
 * Initialize the system clock
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
extern void SystemInit (void);


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

#pragma anon_unions

/*------------- System Control (SYSCON) --------------------------------------*/
typedef struct
{
  __IO uint32_t SYSMEMREMAP;     /* Sys mem. Remap, Offset 0x0   */
  __IO uint32_t PRESETCTRL;
  __IO uint32_t SYSPLLCTRL;      /* Sys PLL control              */
  __IO uint32_t SYSPLLSTAT;
  __IO uint32_t USBPLLCTRL;      /* USB PLL control, offset 0x10 */
  __IO uint32_t USBPLLSTAT;
       uint32_t RESERVED0[2];

  __IO uint32_t SYSOSCCTRL;      /* Offset 0x20 */
  __IO uint32_t WDTOSCCTRL;
  __IO uint32_t IRCCTRL;
       uint32_t RESERVED1[1];
  __IO uint32_t SYSRESSTAT;      /* Offset 0x30 */
       uint32_t RESERVED2[3];
  __IO uint32_t SYSPLLCLKSEL;    /* Offset 0x40 */	
  __IO uint32_t SYSPLLCLKUEN;
  __IO uint32_t USBPLLCLKSEL;
  __IO uint32_t USBPLLCLKUEN;
       uint32_t RESERVED3[8];

  __IO uint32_t MAINCLKSEL;      /* Offset 0x70 */
  __IO uint32_t MAINCLKUEN;
  __IO uint32_t SYSAHBCLKDIV;
       uint32_t RESERVED4[1];

  __IO uint32_t SYSAHBCLKCTRL;   /* Offset 0x80 */
       uint32_t RESERVED5[4];
  __IO uint32_t SSP0CLKDIV;                 
  __IO uint32_t UARTCLKDIV;
  __IO uint32_t SSP1CLKDIV;
       uint32_t RESERVED6[3];
  __IO uint32_t TRACECLKDIV;

  __IO uint32_t SYSTICKCLKDIV;   /* Offset 0xB0 */           
       uint32_t RESERVED7[3];

  __IO uint32_t USBCLKSEL;       /* Offset 0xC0 */ 
  __IO uint32_t USBCLKUEN;
  __IO uint32_t USBCLKDIV;
       uint32_t RESERVED8[1];
  __IO uint32_t WDTCLKSEL;       /* Offset 0xD0 */
  __IO uint32_t WDTCLKUEN;
  __IO uint32_t WDTCLKDIV;
       uint32_t RESERVED9[1];              
  __IO uint32_t CLKOUTCLKSEL;    /* Offset 0xE0 */
  __IO uint32_t CLKOUTUEN;
  __IO uint32_t CLKOUTDIV;              
       uint32_t RESERVED10[5];
  
  __IO uint32_t PIOPORCAP0;      /* Offset 0x100 */           
  __IO uint32_t PIOPORCAP1;         
       uint32_t RESERVED11[18];

  __IO uint32_t BODCTRL;         /* Offset 0x150 */
       uint32_t RESERVED12[1];
  __IO uint32_t SYSTCKCAL;
 	   uint32_t RESERVED13[5];
  __IO uint32_t INT_IRQ_LATENCY; /* Offset 0x170 */
       uint32_t RESERVED14[35];          

  __IO uint32_t STARTAPRP0;      /* Offset 0x200 */     
  __IO uint32_t STARTERP0;             
  __IO uint32_t STARTRSRP0CLR;
  __IO uint32_t STARTSRP0;
  __IO uint32_t STARTAPRP1;           
  __IO uint32_t STARTERP1;             
  __IO uint32_t STARTRSRP1CLR;
  __IO uint32_t STARTSRP1;
       uint32_t RESERVED15[4];

  __IO uint32_t PDSLEEPCFG;      /* Offset 0x230 */
  __IO uint32_t PDAWAKECFG;              
  __IO uint32_t PDRUNCFG;
       uint32_t RESERVED16[110];
  __I  uint32_t DEVICE_ID;
} SYSCON_TypeDef;


/*------------- Pin Connect Block (IOCON) --------------------------------*/
typedef struct
{
  __IO uint32_t PIO2_6;
       uint32_t RESERVED0[1];
  __IO uint32_t PIO2_0;
  __IO uint32_t RESET_PIO0_0;
  __IO uint32_t PIO0_1;
  __IO uint32_t PIO1_8;
       uint32_t RESERVED1[1];
  __IO uint32_t PIO0_2;

  __IO uint32_t PIO2_7;
  __IO uint32_t PIO2_8;
  __IO uint32_t PIO2_1;
  __IO uint32_t PIO0_3;
  __IO uint32_t PIO0_4;
  __IO uint32_t PIO0_5;
  __IO uint32_t PIO1_9;
  __IO uint32_t PIO3_4;

  __IO uint32_t PIO2_4;
  __IO uint32_t PIO2_5;
  __IO uint32_t PIO3_5;
  __IO uint32_t PIO0_6;
  __IO uint32_t PIO0_7;
  __IO uint32_t PIO2_9;
  __IO uint32_t PIO2_10;
  __IO uint32_t PIO2_2;

  __IO uint32_t PIO0_8;
  __IO uint32_t PIO0_9;
  __IO uint32_t JTAG_TCK_PIO0_10;
  __IO uint32_t PIO1_10;
  __IO uint32_t PIO2_11;
  __IO uint32_t JTAG_TDI_PIO0_11;
  __IO uint32_t JTAG_TMS_PIO1_0;
  __IO uint32_t JTAG_TDO_PIO1_1;

  __IO uint32_t JTAG_nTRST_PIO1_2;
  __IO uint32_t PIO3_0;
  __IO uint32_t PIO3_1;
  __IO uint32_t PIO2_3;
  __IO uint32_t ARM_SWDIO_PIO1_3;
  __IO uint32_t PIO1_4;
  __IO uint32_t PIO1_11;
  __IO uint32_t PIO3_2;

  __IO uint32_t PIO1_5;
  __IO uint32_t PIO1_6;
  __IO uint32_t PIO1_7;
  __IO uint32_t PIO3_3;
  __IO uint32_t SCKLOC; 
  __IO uint32_t DSRLOC;
  __IO uint32_t DCDLOC;
  __IO uint32_t RILOC;
} IOCON_TypeDef;


/*------------- Power Management Unit (PMU) --------------------------*/
typedef struct
{
  __IO uint32_t PCON;
  __IO uint32_t GPREG0;
  __IO uint32_t GPREG1;
  __IO uint32_t GPREG2;
  __IO uint32_t GPREG3;
  __IO uint32_t GPREG4;
} PMU_TypeDef;


/*------------- General Purpose Input/Output (GPIO) --------------------------*/
typedef struct
{
  union {
    __IO uint32_t MASKED_ACCESS[4096];
    struct {
         uint32_t RESERVED0[4095];
    __IO uint32_t DATA;
    };
  };
       uint32_t RESERVED1[4096];
  __IO uint32_t DIR;
  __IO uint32_t IS;
  __IO uint32_t IBE;
  __IO uint32_t IEV;
  __IO uint32_t IE;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t IC;
} GPIO_TypeDef;


/*------------- Timer (TMR) --------------------------------------------------*/
typedef struct
{
  __IO uint32_t IR;
  __IO uint32_t TCR;
  __IO uint32_t TC;
  __IO uint32_t PR;
  __IO uint32_t PC;
  __IO uint32_t MCR;
  __IO uint32_t MR0;
  __IO uint32_t MR1;
  __IO uint32_t MR2;
  __IO uint32_t MR3;
  __IO uint32_t CCR;
  __I  uint32_t CR0;
       uint32_t RESERVED1[3];
  __IO uint32_t EMR;
       uint32_t RESERVED2[12];
  __IO uint32_t CTCR;
  __IO uint32_t PWMC;
} TMR_TypeDef;

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
typedef struct
{
  union {
  __I  uint32_t  RBR;
  __O  uint32_t  THR;
  __IO uint32_t  DLL;
  };
  union {
  __IO uint32_t  DLM;
  __IO uint32_t  IER;
  };
  union {
  __I  uint32_t  IIR;
  __O  uint32_t  FCR;
  };
  __IO uint32_t  LCR;
  __IO uint32_t  MCR;
  __I  uint32_t  LSR;
  __I  uint32_t  MSR;
  __IO uint32_t  SCR;
  __IO uint32_t  ACR;
  __IO uint32_t  ICR;
  __IO uint32_t  FDR;
       uint32_t  RESERVED0;
  __IO uint32_t  TER;
       uint32_t  RESERVED1[6];
  __IO uint32_t  RS485CTRL;
  __IO uint32_t  ADRMATCH;
  __IO uint32_t  RS485DLY;
  __I  uint32_t  FIFOLVL;
} UART_TypeDef;

/*------------- Synchronous Serial Communication (SSP) -----------------------*/
typedef struct
{
  __IO uint32_t CR0;
  __IO uint32_t CR1;
  __IO uint32_t DR;
  __I  uint32_t SR;
  __IO uint32_t CPSR;
  __IO uint32_t IMSC;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t ICR;
} SSP_TypeDef;

/*------------- Inter-Integrated Circuit (I2C) -------------------------------*/
typedef struct
{
  __IO uint32_t CONSET;
  __I  uint32_t STAT;
  __IO uint32_t DAT;
  __IO uint32_t ADR0;
  __IO uint32_t SCLH;
  __IO uint32_t SCLL;
  __O  uint32_t CONCLR;
  __IO uint32_t MMCTRL;
  __IO uint32_t ADR1;
  __IO uint32_t ADR2;
  __IO uint32_t ADR3;
  __I  uint32_t DATA_BUFFER;
  __IO uint32_t MASK0;
  __IO uint32_t MASK1;
  __IO uint32_t MASK2;
  __IO uint32_t MASK3;
} I2C_TypeDef;

/*------------- Watchdog Timer (WDT) -----------------------------------------*/
typedef struct
{
  __IO uint32_t MOD;
  __IO uint32_t TC;
  __O  uint32_t FEED;
  __I  uint32_t TV;
} WDT_TypeDef;

/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/
typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t GDR;
       uint32_t RESERVED0;
  __IO uint32_t INTEN;
  __I  uint32_t DR0;
  __I  uint32_t DR1;
  __I  uint32_t DR2;
  __I  uint32_t DR3;
  __I  uint32_t DR4;
  __I  uint32_t DR5;
  __I  uint32_t DR6;
  __I  uint32_t DR7;
  __I  uint32_t STAT;
} ADC_TypeDef;

/*------------- CAN Controller (CAN) ----------------------------*/
typedef struct
{
  __IO uint32_t CNTL;				/* 0x000 */
  __IO uint32_t STAT;
  __IO uint32_t EC;
  __IO uint32_t BT;
  __IO uint32_t INT;
  __IO uint32_t TEST;
  __IO uint32_t BRPE;
       uint32_t RESERVED0;
  __IO uint32_t IF1_CMDREQ;			/* 0x020 */
  __IO uint32_t IF1_CMDMSK;
  __IO uint32_t IF1_MSK1;
  __IO uint32_t IF1_MSK2;
  __IO uint32_t IF1_ARB1;
  __IO uint32_t IF1_ARB2;
  __IO uint32_t IF1_MCTRL;
  __IO uint32_t IF1_DA1;
  __IO uint32_t IF1_DA2;
  __IO uint32_t IF1_DB1;
  __IO uint32_t IF1_DB2;
       uint32_t RESERVED1[13];   
  __IO uint32_t IF2_CMDREQ;			/* 0x080 */
  __IO uint32_t IF2_CMDMSK;
  __IO uint32_t IF2_MSK1;
  __IO uint32_t IF2_MSK2;
  __IO uint32_t IF2_ARB1;
  __IO uint32_t IF2_ARB2;
  __IO uint32_t IF2_MCTRL;
  __IO uint32_t IF2_DA1;
  __IO uint32_t IF2_DA2;
  __IO uint32_t IF2_DB1;
  __IO uint32_t IF2_DB2;
	   uint32_t RESERVED2[21];
  __I  uint32_t TXREQ1;				/* 0x100 */
  __I  uint32_t TXREQ2;
	   uint32_t RESERVED3[6];
  __I  uint32_t ND1;				/* 0x120 */
  __I  uint32_t ND2;
	   uint32_t RESERVED4[6];
  __I  uint32_t IR1;				/* 0x140 */
  __I  uint32_t IR2;
	   uint32_t RESERVED5[6];
  __I  uint32_t MSGV1;				/* 0x160 */
  __I  uint32_t MSGV2;
	   uint32_t RESERVED6[6];
  __IO uint32_t CLKDIV;				/* 0x180 */
} CAN_TypeDef;

/*------------- Universal Serial Bus (USB) -----------------------------------*/
typedef struct
{
  __I  uint32_t DevIntSt;            /* USB Device Interrupt Registers     */
  __IO uint32_t DevIntEn;
  __O  uint32_t DevIntClr;
  __O  uint32_t DevIntSet;

  __O  uint32_t CmdCode;             /* USB Device SIE Command Registers   */
  __I  uint32_t CmdData;

  __I  uint32_t RxData;              /* USB Device Transfer Registers      */
  __O  uint32_t TxData;
  __I  uint32_t RxPLen;
  __O  uint32_t TxPLen;
  __IO uint32_t Ctrl;
  __O  uint32_t DevFIQSel;
} USB_TypeDef;

#pragma no_anon_unions

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
#define FLASH_BASE            (0x00000000UL)
#define RAM_BASE              (0x10000000UL)
#define APB0_BASE             (0x40000000UL)
#define AHB_BASE              (0x50000000UL)

/* APB0 peripherals                                                          */
#define I2C_BASE              (APB0_BASE + 0x00000)
#define WDT_BASE              (APB0_BASE + 0x04000)
#define UART_BASE             (APB0_BASE + 0x08000)
#define CT16B0_BASE           (APB0_BASE + 0x0C000)
#define CT16B1_BASE           (APB0_BASE + 0x10000)
#define CT32B0_BASE           (APB0_BASE + 0x14000)
#define CT32B1_BASE           (APB0_BASE + 0x18000)
#define ADC_BASE              (APB0_BASE + 0x1C000)
#define USB_BASE              (APB0_BASE + 0x20000)
#define PMU_BASE              (APB0_BASE + 0x38000)
#define SSP0_BASE             (APB0_BASE + 0x40000)
#define IOCON_BASE            (APB0_BASE + 0x44000)
#define SYSCON_BASE           (APB0_BASE + 0x48000)
#define CAN_BASE              (APB0_BASE + 0x50000)
#define SSP1_BASE             (APB0_BASE + 0x58000)

/* AHB peripherals */	
#define GPIO_BASE             (AHB_BASE  + 0x00000)
#define GPIO0_BASE            (AHB_BASE  + 0x00000)
#define GPIO1_BASE            (AHB_BASE  + 0x10000)
#define GPIO2_BASE            (AHB_BASE  + 0x20000)
#define GPIO3_BASE            (AHB_BASE  + 0x30000)

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define LPC_I2C                   ((      I2C_TypeDef *)       I2C_BASE)
#define LPC_WDT                   ((      WDT_TypeDef *)       WDT_BASE)
#define LPC_UART                  ((     UART_TypeDef *)      UART_BASE)
#define LPC_TMR16B0               ((      TMR_TypeDef *)    CT16B0_BASE)
#define LPC_TMR16B1               ((      TMR_TypeDef *)    CT16B1_BASE)
#define LPC_TMR32B0               ((      TMR_TypeDef *)    CT32B0_BASE)
#define LPC_TMR32B1               ((      TMR_TypeDef *)    CT32B1_BASE)
#define LPC_ADC                   ((      ADC_TypeDef *)       ADC_BASE)
#define LPC_PMU                   ((      PMU_TypeDef *)       PMU_BASE)
#define LPC_SSP0                  ((      SSP_TypeDef *)      SSP0_BASE)
#define LPC_SSP1                  ((      SSP_TypeDef *)      SSP1_BASE)
#define LPC_CAN                   ((      CAN_TypeDef *)       CAN_BASE)
#define LPC_IOCON                 ((    IOCON_TypeDef *)     IOCON_BASE)
#define LPC_SYSCON                ((   SYSCON_TypeDef *)    SYSCON_BASE)
#define LPC_USB                   ((      USB_TypeDef *)       USB_BASE)

#define LPC_GPIO0                 ((     GPIO_TypeDef *)     GPIO0_BASE)
#define LPC_GPIO1                 ((     GPIO_TypeDef *)     GPIO1_BASE)
#define LPC_GPIO2                 ((     GPIO_TypeDef *)     GPIO2_BASE)
#define LPC_GPIO3                 ((     GPIO_TypeDef *)     GPIO3_BASE)

#endif  // __LPC11xx_H__
