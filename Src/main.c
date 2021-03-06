/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "stm32f4xx.h"


#define PCLK 			16000000
#define ADDR			7
#define I2C_START   		(1<<8)
#define I2C_STOP			(1<<9)
#define I2C_SR_SB       (1<<0)
#define I2C_SR_ADDR     (1<<1)
#define I2C_SR_TxE      (1<<7)
#define I2C_SR_BTF      (1<<2)
#define I2C_FSCL        100000
#define I2C_CR1_PE      1<<0
#define I2C_CR1_SWRST   1<<15
#define RXNE			1<<8
#define CS43L22_ADDRESS 0x94U
#define CS43L22_REG_ID  0x01
#define CS43L22_CHIP_ID 0x1C

volatile uint8_t DeviceAddr = CS43L22_ADDRESS;

uint32_t  DATA=0;
uint8_t x;
uint8_t buff[100];

/**********************************************************************************************************/

int main(void)
   {
	//uint8_t timestamp;
	//float temp;
	gpio_init();
	pwm_init();
	ADC_init_TIMER_TRIGER();

	UART_Rx_TX_Init();
	spi_init();

	adc_start();

	SPI1->CR1|=SPE;
	int16_t rbuf[4];

	spi_write(0x20, 0x67U);

	spi_write(0x24U, 0x48U);

    for(int i=0; i<10000000; i++);

    rbuf[0]=(uint8_t)spi_read(0x0f);



while(1)
	{

	DATA=adc_get_data();

    uint8_t  temp=DATA/RES*VREF;
    USART_WRITE(temp);
	tim_oc_init();

	rbuf[1]=spi_read((uint8_t) 0x29U);
	rbuf[2]=spi_read((uint8_t) 0x2BU);
	rbuf[3]=spi_read((uint8_t) 0x2DU);

	GPIOD->BSRR=(1<<12);
	for(int i=0;i<100000;i++) {}
    GPIOD->BSRR=(1<<28);
	for(int i=0;i<100000;i++) {}
    }


}
void gpio_init()
{
	//ENABLE THE PERIPHERAL CLOCK FOR GPIOD
    GPIOA_PCLK_EN();
    GPIOD_PCLK_EN() ;
    //set the moder reg
	GPIOD->MODER|=(1<<24);
	GPIOA->MODER|=(1<<0);
}
/*********************************************************************************************************/

/*     PA5 - SPI1 SCK
 *     PA6 - SPI1 MISO
 *     PA7 - SPI1 MOSI
 *     PE3 - SPI CS
 */
void spi_init()
{
	//configure pins alternate mode and choose the    correct AF number (5 for SPI1)
		/*     PA5 - SPI1 SCK
		 *     PA6 - SPI1 MISO
		 *     PA7 - SPI1 MOSI
		 *     PE3 - SPI CS
		 */	// GPIOE->MODER &= 0xFFFFFF3F; // reset bits 6-7
	 SPI1_PCLK_EN() ;
	 GPIOA_PCLK_EN();
	 GPIOD_PCLK_EN() ;
	 GPIOE_PCLK_EN();

	 GPIOE->MODER |= 1<<3*2; // set bits 6-7 to 0b01 (output)
	 GPIOE->ODR |= (1 << 3);

	 GPIOA->OSPEEDR |= 0x0000FC00;
	 GPIOA->MODER|=(2<<10);
	 GPIOA->MODER|=(2<<12);
	 GPIOA->MODER|=(2<<14);
	 GPIOA->AFR[0] |= (0x5 << 20); 			// for pin 5
	 GPIOA->AFR[0] |= (0x5 << 24);			// for pin 6
	 GPIOA->AFR[0] |= (0x5 << 28); 			// for pin 7


	 GPIOD->MODER &= 0x00FFFFFF;
	 GPIOD->MODER |= 0x55000000;
	 GPIOD->ODR    = 0;


	SPI1->CR1|=(2<<2);					// 1. set BR[2:0] for serial clock baud rate (SPI_CR1)
	SPI1->CR1|=SPI_16BIT;				//2. set DFF bit for 8/16 bit data format
	SPI1->CR1|=CPOL_HIGH;				// 3. select CPOL and CPHA bits for data/clock relationship
	SPI1->CR1|=CPHA_SECOND;				//2ndST RISING EDGE

	//SPI1->CR1|=SPI_MSB;				//4.MSBFIRST bit for frame format

	SPI1->CR1|=SPI_SSM;					//5. set SSM bit for software slave management
    SPI1->CR1|=SPI_SSI;					//6. set SSI bit (important step)
	SPI1->CR1|=SPI_MSTR;				// 8. choose master mode MSTR
}



uint16_t spi_read(uint32_t reg)
{
	 uint32_t temp;
	 GPIOE->ODR&=(0 << 3);//ENABLE THE SLAVE SELECT LINE
	 uint16_t frame = 0;
	 frame |= (uint16_t)(reg << 8);
	 frame |= (1 << 15); // read bit
	 while(!(SPI1->SR&SPI_TXE)){ }
     SPI1->DR=frame;
	 while(!(SPI1->SR&SPI_RXNE))
	 { }
	 temp =SPI1->DR;
   	 while((SPI1->SR& 1<<7))
   	 {

   	 }

	 GPIOE->ODR|=(1<< 3);

	 return temp;
}

void spi_write(uint8_t reg, uint8_t data)
{
	GPIOE->ODR&=(0 << 3); // enabl THE SLAVE SELECT LINE

	uint32_t frame = 0;
	frame = data;
	frame |= (uint16_t)(reg << 8);

	SPI1->DR=frame;
	while(!(SPI1->SR&SPI_TXE))
     {

     }
	while(!(SPI1->SR&SPI_RXNE))
     {

     }
	(void)SPI1->DR;//dummy

	while((SPI1->SR& 1<<7))
     {}
    GPIOE->ODR|=(1<< 3);

}


void i2c_init()
{

      //controlling the dac via i2c1
	  //PB6  --->  SCL
	  //PB9  --->  SDA
	  GPIOB_PCLK_EN();
	  I2C1_PCLK_EN();										//enable the peripheral clock for the i2c
	  GPIOD_PCLK_EN();							//enable the peripheral clock for the GPIOD


	  //CONFIGURE THE LED

	  GPIOD->MODER &= ~(0xFFU << 24);
	  GPIOD->MODER |= (0x55 << 24);
	  GPIOD->ODR    = 0x0000;

	  //CONFIGURE THE I2C PIN

      GPIOB->MODER|=(2<<12)|(2<<18);
      GPIOB->AFR[0]|=(4<<24);
      GPIOB->AFR[1]|=(4<<4);

      GPIOB->OTYPER|=(1<<6)|(1<<9);							//select the open drain output type
      GPIOB->PUPDR|=(1<<12)|(1<<18);                        //enable the internal pull up


      I2C1->CR1 = I2C_CR1_SWRST;                            //SOFTWARE RESET AND CLEAR HTE REG
      I2C1->CR1 = 0;


     // I2C1->CR1|=1<<9;										//ENABLE THE ACK





      // fPCLK1 must be at least 2 Mhz for SM mode
      // must be at least 4 Mhz for FM mode
      // must be multiple of 10Mhz to reach 400 kHz
      // DAC works at 100 khz (SM mode)
      // For SM Mode:
      // Thigh = CCR * TPCLK1
      // Tlow  = CCR * TPCLK1
      // So to generate 100 kHz SCL frequency
      // we need 1/100kz = 10us clock speed
      // Thigh and Tlow needs to be 5us each
      // Let's pick fPCLK1 = 10Mhz, TPCLK1 = 1/10Mhz = 100ns
      // Thigh = CCR * TPCLK1 => 5us = CCR * 100ns
      // CCR = 50
      I2C1->CR2|=10<<0;                            //SELECT THE PERIPHERAL CLOCK REQUENCY
      I2C1->CCR|=50<<0;                            //SET THE CCR VLUE FOR SM N FM FSCL IS THE SPEED OF SCLK


      // Maximum rise time.
      // Calculation is (maximum_rise_time / fPCLK1) + 1
      // In SM mode maximum allowed SCL rise time is 1000ns
      // For TPCLK1 = 100ns => (1000ns / 100ns) + 1= 10 + 1 = 11

      I2C1->TRISE|(11<<0);

      I2C1->OAR1|=(0x00 << 1); 						//INTERFACE ADDRESS  NOT USED IN HERE
      I2C1->OAR1|=1<<14;    				        //BIT SHOULD ALWAYS BE KEPT 1

      I2C1->CR1 |= I2C_CR1_PE;                       // enable i2c

      //configure the audio coec

      GPIOD->MODER&=~(3<<8);						//CLEAR THE BIT
      GPIOD->MODER|=(1<<8); 						//set to gpio ouput

      GPIOD->ODR|=(1<<4);							//activate the cs43l22






}


void i2c_write(uint8_t regaddr, uint8_t data)
{



	I2C1->CR1|=(1<<8);									    //GENERATE THE START CONDITION

	while(!(I2C1->SR1& I2C_SR_SB)){}						//CHECK WETHER START GENERATION OCCURED OR NOT

	/*DeviceAddr=DeviceAddr<<1;							    //SHIFT THE 7 BIT TO SET R/W BIT

	DeviceAddr&=~(1);										 //0 FOR WRITE OPERATION*/

	I2C1->DR=DeviceAddr;								   //SEND THE SLAVE ADDR
    while(I2C1->SR1 & I2C_SR_ADDR){}						//CHECK FOR THE ADDR FLAG TO
    uint32_t dummy =I2C1->SR1;							    //CLEAR THR ADDR FLAG BY READING THE  SR1 AND SR2 FLAG
    dummy =I2C1->SR2;
    (void)dummy;

    I2C1->DR = regaddr;									   //send the sub register address
    while(!(I2C1->SR1&I2C_SR_TxE)){}
    while(!(I2C1->SR1&I2C_SR_BTF)){}

    I2C1->DR=data;											//send the data

    while(!(I2C1->SR1&I2C_SR_TxE)){}
    while(!(I2C1->SR1&I2C_SR_BTF)){}

    /*
    while(len>0)
    {
    	while(!(I2C1->SR1&I2C_SR_TxE)){}					//WAIT FOR THE TXE
    	I2C1->DR=*Data++;
    	len--;

    }
    //ONCE LEN BECOME 0 WAIT FOR TC=XE AND BTF TO SET
    while(!(I2C1->SR1&I2C_SR_TxE)){}
    while(!(I2C1->SR1&I2C_SR_BTF)){}
    */

    I2C1->CR1|=(1<<9);									//GENERATE THE STOP CONDITION



}

uint8_t i2c_Read(uint8_t regaddr)
{
	uint8_t reg;

	I2C1->CR1|=(1<<8);									    //GENERATE THE START CONDITION
    while(!(I2C1->SR1& I2C_SR_SB)){}						//CHECK WETHER START GENERATION OCCURED OR NOT



    I2C1->DR=DeviceAddr;									//SEND THE SLAVE ADDR 8th bit is 0
    while(I2C1->SR1 & I2C_SR_ADDR){}						//CHECK FOR THE ADDR FLAG TO
    uint32_t dummy =I2C1->SR1;							    //CLEAR THR ADDR FLAG BY READING THE  SR1 AND SR2 FLAG
    dummy =I2C1->SR2;
    (void)dummy;

    I2C1->DR = regaddr;
    while(!(I2C1->SR1&I2C_SR_TxE)){}
    while(!(I2C1->SR1&I2C_SR_BTF)){}

    //stop and restart the communication
    I2C1->CR1|=(1<<9);									    //GENERATE THE STOP CONDITION
    I2C1->CR1|=(1<<8);									    //GENERATE THE START CONDITION

    I2C1->DR=DeviceAddr|0X01;							    //SEND IN READ MODE
    while(I2C1->SR1 & I2C_SR_ADDR){}						//CHECK FOR THE ADDR FLAG TO
    dummy =I2C1->SR1;							    //CLEAR THR ADDR FLAG BY READING THE  SR1 AND SR2 FLAG
    dummy =I2C1->SR2;
    (void)dummy;

    // wait until receive buffer is not empty
    while(!(I2C1->SR1& RXNE)){}
    reg = (uint8_t)I2C1->DR;
    I2C1->CR1|=(1<<9);									    //GENERATE THE STOP CONDITION

    return reg;
}



void tim_oc_init()
{
	//TOGLING LD14 LD15 USING OUTPUT COPARE
    //enable the clock to the tim4
	GPIOD_PCLK_EN() ;
	GPIOD->MODER|=(2<<14*2);
	GPIOD->AFR[1]|=(2<<24);

	TIM4_PERIPHERAL_CLK_ENABLE();

    TIM4->PSC|=160000-1;									//slect the prescaler

    TIM4->ARR=1000;

    TIM4->CCMR2|=0x30;										//toggle on match

    TIM4->CCR[3]=400;

    TIM4->CCER|=(1<<8);

    TIM4->CNT=0;

    TIM4->CR1|=(1<<0);

}


void tim_IC_INIT()
{
	GPIOA_PCLK_EN();
	GPIOA->MODER|=(2<<16);//ALT FUNC FOR TIM1
	GPIOA->AFR[1]=(1<<0);

	TIM1_PERIPHERAL_CLK_ENABLE();						//I AM CONNECTING THE PA0 AS INPUT TO TIM1->PA8 AF1,

	TIM1->PSC|=16000000;								//SET THE PRESCALER

    TIM1->CCMR1|=(1<<0);								//SELECTING THE CHANEL 1 AS THE INPUT CAPTURE

    TIM1->CCER|=(1<<0);							    	//ENABLE THE CAPTURE COPMARE BY CONFIGURONG THE CCER

    TIM1->CR1|=(1<<0);                                  //ENABLE THE TIMER
}


static void UART_Rx_TX_Init(void)
{
	GPIOA_PCLK_EN();
	GPIOA->MODER|=(2<<2*2)|(2<<3*2);
	GPIOA->AFR[0]|=(7<<2*4)|(7<<3*4);

	USART2_PCLK_EN();									//enble the peripheral clock

	USART2->BRR|=((SYSTEMCLK+BAUDRATE/2)/BAUDRATE);		//set the baudrate

	USART2->CR1=CR1_RE|CR1_TE;							//Enable the uart RX and TX

	USART2->CR1|=CR1_UE;								//ENABLE THE UART

}

static void USART_WRITE(uint8_t ch)
{

	while(!(USART2->SR&SR_TXE)){}						//wait till the uart tx data register is empty

	USART2->DR=(ch&0xFF);								//write the data to the data register

}

char read(void)
{

	while(!(USART2->SR&SR_RXNE)){}						/*make sure receive data registr not empty*/

	return USART2->DR;									/*Read the data*/
}


void pwm_init()
{

	GPIOD_PCLK_EN() ;
	GPIOD->MODER|=(2<<30);
	GPIOD->AFR[1]|=(2<<28);
	TIM4_PERIPHERAL_CLK_ENABLE();
	TIM4->PSC|=10-1;						        	//slect the prescaler
	TIM4->ARR=26667-1;
	TIM4->CNT=0;
	TIM4->CCMR2|=6<<12;					        		//toggle on match
    TIM4->CCER|=(1<<12);
	TIM4->CCR[3]=8889-1;
	TIM4->CR1|=(1<<0);
}
void ADC_init_TIMER_TRIGER()
{

	GPIOD_PCLK_EN();
	GPIOD->MODER|=(2<<30);
	GPIOD->AFR[1]|=(2<<28);

	TIM1_PERIPHERAL_CLK_ENABLE();				//I AM CONNECTING THE PA0 AS INPUT TO TIM1->PA8 AF1,


	TIM1->PSC|=16;								//SET THE PRESCALER
	TIM1->ARR|=100;

	TIM1->CR2&=~(1<<4);
	TIM1->CR2|=(2<<4);							//UPDATE EVENT
	TIM1->CR1|=(1<<0);							//ENABLE THE TIMER

	//adc1 ch1 is connected to PA1
	ADC1_PERIPHERAL_CLK_ENABLE();				//enable the periph clock

	ADC1->CR2|=(1<<1);							//continous conversion mode
	ADC1->SMPR[0]|=3<<0;						//SAMPLING TIME 480 CYCLES
	ADC1->SQR[2]=(1<<0);						//INITIATIN THE SEQUENCING  secleting the chanel 1
	ADC1->SQR[0]=0x00;							//specifying the no of conversion for the single chanel


	ADC1->CR2|=(1<<28);							//external trigger enable
	ADC1->CR2|=(0<<24)|(0<<25)|(0<<26)|(0<<27);	//external trigger event selection
	ADC1->CR2|=1<<0;							//ADC ON

}



void ADC_init()
{

	GPIOA_PCLK_EN();
	GPIOA->MODER|=3<<2;

	ADC1_PERIPHERAL_CLK_ENABLE();				//adc1 ch1 is connected to PA1
	//ADC1->CR1|=(1<<8);//scan mode
	ADC1->CR2|=(1<<1);							//continous conversion mode
	ADC1->SMPR[0]|=3<<0;						//SAMPLING TIME 480 CYCLES
	ADC1->SQR[2]=(1<<0);						//INITIATIN THE chanel 1 SEQUENCING
	ADC1->SQR[0]=0x00;							//selceting the no of conversion
	ADC1->CR2|=1<<0;							//ADC ON

}

void adc_start()
{
	ADC1->CR2|=1<<30;    						//START THE CONVERSION
}


uint32_t adc_get_data()
{
	while(!(ADC1->SR&(1<<1))){}                 //wait for EOC flag to set
    return ADC1->DR;                            //rea the data

}


void dac_init()
{
	//DAC1 OUT IS CONNECTED TO PA4
	//DAC1 OUT IS CONNECTED TO PA4






}
/****************************************************************************************************************/























//

	  /*
	   *
	   *
	   *
	   *
	   *
	   *
	   *
	   *
	   *
	   	   /*
	read_buf[0] =(uint8_t) spi_read(LIS302_REG_OUT_X);
	read_buf[1] =(uint8_t) spi_read(LIS302_REG_OUT_Y);
	read_buf[2] =(uint8_t) spi_read(LIS302_REG_OUT_Z);

	if (read_buf[0] > 16) {
	            GPIOD->ODR &= (uint16_t)~0x2000;
	            GPIOD->ODR |= 0x8000;
	        } else if (read_buf[0] < -16 ) {
	            GPIOD->ODR &= (uint16_t)~0x8000;
	            GPIOD->ODR |= 0x2000;
	        } else {
	            GPIOD->ODR &= (uint16_t)~0xA000;
	        }
	 if (read_buf[1] > 16) {
	            GPIOD->ODR &= (uint16_t)~0x1000;
	            GPIOD->ODR |= 0x4000;
	        } else if (read_buf[1] < -16 ) {
	            GPIOD->ODR &= (uint16_t)~0x4000;
	            GPIOD->ODR |= 0x1000;
	        } else {
	            GPIOD->ODR &= (uint16_t)~0x5000;
	        }
	   *
	   *
	   *timestamp=TIM1->CCR[0];*/

