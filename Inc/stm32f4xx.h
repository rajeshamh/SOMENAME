/*
 * stm32f4xx.h
 *
 *  Created on: 28-Oct-2021
 *      Author: Rajesha M H
 */

#ifndef STM32F4XX_H_
#define STM32F4XX_H_


#define PERIPHERAL_BASEADDR              (0x40000000U)
#define AHB1PERIPH_BASEADDR		         (0x40020000U)
#define AHB2PERIPH_BASEADDR 			 (0x50000000U)
#define APB1PERIPH_BASEADDR				 (0x40000000U)
#define APB2PERIPH_BASEADDR	             (0x40010000U)

#define	GPIOB_BASEADDR		             (AHB1PERIPH_BASEADDR + 0X0400)
#define ADC1_BASEADDR                    (APB2PERIPH_BASEADDR + 0X2000)
#define	GPIOD_BASEADDR		             (AHB1PERIPH_BASEADDR + 0X0C00)
#define	GPIOA_BASEADDR					 (AHB1PERIPH_BASEADDR + 0X0000)
#define GPIOE_BASEADDR		             (AHB1PERIPH_BASEADDR + 0X1000)
#define RCC_BASEADDR       				 (AHB1PERIPH_BASEADDR + 0x3800)
#define SPI1_BASEADDR				     (APB2PERIPH_BASEADDR + 0X3000)
#define TIM1_BASEADDR					 (APB2PERIPH_BASEADDR + 0X0000)
#define TIM4_BASEADDR					 (APB1PERIPH_BASEADDR + 0x0800)
#define USART2_BASEADDR		             (APB1PERIPH_BASEADDR + 0X4400)
#define I2C1_BASEADDR                    (APB1PERIPH_BASEADDR + 0X5400)

#define I2C1  	                         ((I2C_RegDef_t*)I2C1_BASEADDR)
#define ADC1							 ((ADCx_Regdef_t*)ADC1_BASEADDR)
#define GPIOA  							 ((GPIO_Regdef_t*)GPIOA_BASEADDR)
#define	GPIOD							 ((GPIO_Regdef_t*)GPIOD_BASEADDR)
#define GPIOE		                     ((GPIO_Regdef_t*)GPIOE_BASEADDR)
#define RCC 							 ((RCC_Regdef_t*)RCC_BASEADDR)
#define TIM1							 ((Timx_Regdef_t*)TIM1_BASEADDR)
#define TIM4                             ((Timx_Regdef_t*)TIM4_BASEADDR)
#define USART2 		 		             ((USART_Regdef_t*)USART2_BASEADDR)
#define SPI1  		                     ((SPI_Regdef_t*)SPI1_BASEADDR)
#define GPIOB		                     ((GPIO_Regdef_t*)GPIOB_BASEADDR)

#define GPIOB_PCLK_EN()		             (RCC->AHB1ENR |= (1<<1))
#define I2C1_PCLK_EN()  	             (RCC->APB1ENR |= (1<<21))
#define ADC1_PERIPHERAL_CLK_ENABLE()	 (RCC->APB2ENR |= (1<<8))
#define	GPIOE_PCLK_EN()		             (RCC->AHB1ENR |= (1<<4))
#define GPIOA_PCLK_EN()    				 (RCC->AHB1ENR |= (1<<0))
#define GPIOD_PCLK_EN()   				 (RCC->AHB1ENR |= (1<<3))
#define SPI1_PCLK_EN()  	             (RCC->APB2ENR |= (1<<12))
#define TIM1_PERIPHERAL_CLK_ENABLE()	 (RCC->APB2ENR |= (1<<0))
#define TIM4_PERIPHERAL_CLK_ENABLE()	 (RCC->APB1ENR |= (1<<2))
#define USART2_PCLK_EN()  	             (RCC->APB1ENR |= (1<<17))

typedef struct
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t OAR1;
  volatile uint32_t OAR2;
  volatile uint32_t DR;
  volatile uint32_t SR1;
  volatile uint32_t SR2;
  volatile uint32_t CCR;
  volatile uint32_t TRISE;
  volatile uint32_t FLTR;
}I2C_RegDef_t;


typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t I2SCFGR;
	volatile uint32_t I2SPR;

}SPI_Regdef_t;



typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GPTR;


}USART_Regdef_t;


typedef struct
{
	volatile uint32_t SR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMPR[2];
	volatile uint32_t JOFR[4];
	volatile uint32_t HTR;
	volatile uint32_t LTR;
	volatile uint32_t SQR[3];
	volatile uint32_t JSQR;
	volatile uint32_t JDR[4];
	volatile uint32_t DR;
	volatile uint32_t CSR;
	volatile uint32_t CCR;
	volatile uint32_t CDR;

}ADCx_Regdef_t;

typedef struct
{


	volatile	uint32_t	CR1;
	volatile   	uint32_t    CR2;
	volatile	uint32_t	SMCR;
	volatile	uint32_t	DIER;
	volatile	uint32_t	SR;
	volatile	uint32_t	EGR;
	volatile	uint32_t	CCMR1;
	volatile	uint32_t	CCMR2;
	volatile	uint32_t	CCER;
	volatile	uint32_t	CNT;
	volatile    uint32_t	PSC;
	volatile 	uint32_t	ARR;
	volatile	uint32_t	RCR;
	volatile	uint32_t	CCR[4];
	volatile	uint32_t	BDTR;
	volatile    uint32_t	DCR;
	volatile	uint32_t	DMAR;


}Timx_Regdef_t;

typedef struct
{

	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	         uint32_t Reserved;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	         uint32_t RESERVED1;
	         uint32_t RESERVED2;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
             uint32_t RESERVED3;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    	     uint32_t RESERVED4;
	         uint32_t RESERVED5;
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	         uint32_t RESERVED6;
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    		 uint32_t RESERVED7;
	         uint32_t RESERVED8;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
	         uint32_t RESERVED9;
	         uint32_t RESERVED10;
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    volatile uint32_t PLLSAICFGR;
    volatile uint32_t DCKCFGR;
}RCC_Regdef_t;


typedef struct
{
	volatile uint32_t MODER;				/*|<TODO	        Address Offset     0x00    */
	volatile uint32_t OTYPER;				/*|<TODO	        Address Offset     0x04    */
	volatile uint32_t OSPEEDR;				/*|<TODO	        Address Offset     0x08    */
	volatile uint32_t PUPDR;				/*|<TODO	        Address Offset     0x1c    */
	volatile uint32_t IDR;					/*|<TODO	        Address Offset     0x10    */
	volatile uint32_t ODR;			    	/*|<TODO	        Address Offset     0x14    */
	volatile uint32_t BSRR;					/*|<TODO	        Address Offset     0x18    */
	volatile uint32_t LCKR;                	/*|<TODO	        Address Offset     0x2c    */
	volatile uint32_t AFR[2];				/*|<TODO	        Address Offset     0x20    */


}GPIO_Regdef_t;








#endif /* STM32F4XX_H_ */
