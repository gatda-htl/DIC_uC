/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Author: Gatt David, Class: 5bHEL, Year: 2020/2021, Subject: DIC, Supervisor: Lezuo
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Project: RS485 stuff, Fileuse: Headerfile for Mainprogram <name.c> for a simplified use
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
#include <stdint.h>

//Peripherals
#define APB ((uint32_t*) 0x40000000)
#define APB2 ((uint32_t*) 0x40010000)
#define AHB1 ((uint32_t*) 0x40020000)
#define AHB2 ((uint32_t*) 0x48000000)

//NVIC
#define NVIC_ISER ((uint32_t*) 0x00) //S.71

//AHB2
#define GPIOA (AHB2+0x00) //0x00 weil Startadresse 

//APB
#define USART1 (APB+0x0013800)

//USART1 S640 Reference Manual stm32f030
#define USART_CR1 (USART1+0x00)
#define USART_CR2 (USART1+0x04)
#define USART_CR3 (USART1+0x08)
#define USART_BRR (USART1+0x0C)
#define USART_TDR (USART1+0x28) 
#define USART_ISR (USART1+0x1C)
#define USART_ICR (USART1+0x20)
//There are many more Registeres which are not needed "I think" I will leave them out for now 

//USART flags
#define USART_ISR_TC ((uint32_t*)0x40)
#define USART_ISR_TXE ((uint32_t*)0x80)
#define USART_ICR_TCCF ((uint32_t*)0x40)

//Alternate Function
#define GPIOA_MODER (GPIOA + 0x00)
#define GPIOA_AFRH (GPIOA + 0x24)
#define GPIOA_ODR (GPIOA+ 0x14)

//clock
#define RCC (AHB1 + 0x1000)//Seite 39 
#define RCC_CFGR3 (RCC + 0x30) //Seite 123 
#define RCC_AHBENR (RCC + 0x14) //Seite 111  
#define RCC_CR (RCC + 0x00)//Seite 123 
#define RCC_APB2ENR (RCC + 0x18)//Seite 112 

//NVIC
#define USART_RQR (USART1+0x18) //S. 634 clear RXNE

//ADC
#define ADC (APB+0x12400)

#define ADC_ISR (ADC + 0x00)
#define ADC_IER (ADC + 0x04)
#define ADC_CR (ADC + 0x08)
#define ADC_CFGR1 (ADC + 0x0C)
#define ADC_CFGR2 (ADC + 0x1)
#define ADC_CHSELR (ADC + 0x28)
#define ADC_DR (ADC + 0x40)

//ADC flags
#define ADC_ISR_EOC ((uint32_t*) 0x4)
#define ADC_ISR_ADRDY ((uint32_t*) 0x1)
#define ADC_DR_DATA ((uint32_t*) 0xFFFF)

