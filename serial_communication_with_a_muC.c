/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Author: Gatt David, Class: 5bHEL, Year: 2020/2021, Subject: DIC, Supervisor: Lezuo
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Project: RS485 stuff, Fileuse: Mainprogram to configure configure send and receive data
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "Adress.h" //Adressregister

int iLauf;
char *usart_data;
//Usartkonfig
void usartstart()
{
    uint32_t reg_content;
    uint32_t *usart1_cr1 = USART_CR1;
    uint32_t *usart1_brr = USART_BRR;

    reg_content = *usart1_cr1;
    reg_content |= 0x6EC; //0000 0000 0000 0000 0000 0110 1110 1100 Erklärung in Dokumentation -> 6EC
    *usart1_cr1 = reg_content;

    reg_content = *usart1_brr = USART_BRR;
    reg_content |= 0xD0; //0000 0000 0000 0000 0000 0000 1101 0000 //208 USARTDIV -> D0
    *usart1_brr = reg_content;

    reg_content = *usart1_cr1;
    reg_content |= 0x1; //0000 0000 0000 0000 0000 0000 0000 0001 // 1 Usart enable
    *usart1_cr1 = reg_content;
}

//Pinkonfig 
void Pinconfig()
{
    uint32_t reg_content;
    uint32_t *gpioa1_moder = GPIOA_MODER;
    uint32_t *gpioa1_afrh = GPIOA_AFRH;

    reg_content = *gpioa1_moder;
    reg_content |= 0x285C00; //00 00 00 01 01 10 10 00 00 00 00 00 00 00 00 00 Erklärung Dokumentation
    *gpioa1_moder = reg_content;

    reg_content = *gpioa1_afrh;
    reg_content |= 0x110; //S. 141 Pin10 und 9 -> AF1 -> 0001  -> 0000 0000 0000 0000 0000 0001 0001 0000 -> 110h
    gpioa1_afrh = reg_content;
}

//Clock enablen und USART Clock Multiplexer umschalten
void Clockconfig() //KOPIERT ÜBERARBEITEN!!!!!!!
{
    uint32_t *usart1_rcc_ahbenr = RCC_AHBENR;
    uint32_t *usart1_rcc_cfgr3 = RCC_CFGR3; 
    uint32_t *usart1_rcc_apb2enr = RCC_APB2ENR;
    uint32_t *usart1_rcc_cr = RCC_CR;
    uint32_t reg_content;

    reg_content = *usart1_rcc_ahbenr;  //Enable GPIOA Clock
    reg_content |= 0x00020000;
    *usart1_rcc_ahbenr = reg_content;

    reg_content = *usart1_rcc_cfgr3;  //Konfiguration SW1
    reg_content |= 0x00000003;
    *usart1_rcc_cfgr3 = reg_content;

    reg_content = *usart1_rcc_apb2enr;  //enable USART1 Clock
    reg_content |= 0x00004000;
    *usart1_rcc_apb2enr = reg_content;

    reg_content = *usart1_rcc_cr;  //enablen der HSI
    reg_content |= 0x00000001;
    *usart1_rcc_cr = reg_content;    
}

void ADCconfig()
{
    uint32_t *adc_ier = ADC_IER;
    uint32_t *adc_cr = ADC_CR;
    uint32_t *adc_cgfr2 = ADC_CFGR2;
    uint32_t *adc_chselr = ADC_CHSELR;
    uint32_t reg_content;

    
    reg_content = *adc_cgfr2;
    reg_content |= 0x80000000;
    *adc_cgfr2 = reg_content;

    reg_content = *adc_cr;  
    reg_content |= 0x80000000;
    *adc_ier = reg_content;
    while ((* adc_cr & 0x80000000 ) == 0x80000000);

    reg_content = *adc_ier;  
    reg_content |= 0x5;
    *adc_ier = reg_content;

    reg_content = *adc_chselr;
    reg_content |= 0x20;
    *adc_chselr = reg_content;

    reg_content = *adc_cr;
    reg_content |= 0x1;
    *adc_cr = reg_content;



}

void NVICconfig()
{
    uint32_t *nvic_iser = NVIC_ISER;
    uint32_t reg_content;

    reg_content = *nvic_iser;
    reg_content |= 0x8000000;
    *nvic_iser = reg_content;

}


void ADC1_IRQHandler(void)
{
    uint32_t *adc_cr = ADC_CR;          //Control Register Used to Start ADC conversion
    uint32_t *adc_isr = ADC_ISR;        //to check if ADC is ready
    uint32_t *adc_dr = ADC_DR;          //ADC Daten

    uint32_t *gpioa_odr = GPIOA_ODR;    //Pins to set the MAX485
    uint32_t *usart_tdr = USART_TDR;    //to write something in the data register of the USART to start an USART Interrupt

    uint32_t adc_isr_eoc = ADC_ISR_EOC;     //Bitmask to check if conversation is finished
    uint32_t adc_isr_adrdy = ADC_ISR_ADRDY;     //Bitmask to check if ADC is ready
    uint32_t adc_dr_data = ADC_DR_DATA;     //Bitmask to check the ADC data register

    uint32_t reg_content;       //value to configur
    uint32_t value_adc;         //value from the ADC Data Register

    if ((*adc_isr & adc_isr_adrdy)== adc_isr_adrdy) //check if ADC is ready if ADC is ready start conversion 
    {
        reg_content = *adc_cr;
        reg_content |= 0x4; //Start ADC conversion
        *adc_cr = reg_content;
    }

    if((*adc_isr & adc_isr_eoc)== adc_isr_eoc)  //check if conversion is finished
    {
        iLauf = 0;
        value_adc = (*adc_dr & adc_dr_data);    //read value of ADC Data register

        reg_content = *gpioa_odr;   //set MAX485 in sending state
        reg_content |= 0xC0;
        *gpioa_odr = reg_content;

        sprintf(usart_data, "%d", value_adc);   //sprintf is used to write something of Usart (KSN 4. Kl)
        *usart_tdr = 'g';

    }

}




//Interrupt 
void USART1_IRQHandler()
{
    uint32_t *usart_isr = USART_ISR;
    uint32_t *usart_icr = USART_ICR;
    uint32_t *usart_tdr = USART_TDR;
    uint32_t *gpioa_odr = GPIOA_ODR;

    uint32_t usart_isr_tc = USART_ISR_TC;
    uint32_t usart_isr_txe = USART_ISR_TXE;
    uint32_t usart_icr_tccf = USART_ICR_TCCF;

    uint32_t reg_content;

    if ((*usart_isr & usart_isr_tc)==usart_isr_tc)
    {
        if(iLauf == sizeof(usart_data))
        {
            reg_content = *gpioa_odr;
            reg_content |= 0x00;
            *gpioa_odr = reg_content;
            iLauf = 0;
            *usart_icr |= usart_icr_tccf;
        }
    

        else
        {
            *usart_tdr = usart_data[iLauf++];
        }
    }
}

int main()
{
    Clockconfig();
    Pinconfig();
    usartstart();
    NVICconfig();
    ADCconfig();    
    

return(0);
}