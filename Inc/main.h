/*
 * main.h
 *
 *  Created on: 28-Oct-2021
 *      Author: Rajesha M H
 */

#ifndef MAIN_H_
#define MAIN_H_


#define VREF 		3.3
#define RES  		4096
#define BAUDRATE	9600
#define SYSTEMCLK	16000000

#define CR1_RE		(1<<2)
#define CR1_TE		(1<<3)
#define CR1_UE		(1<<13)
#define SR_TXE		(1<<7)
#define SR_RXNE     (1<<5)

#define SPI_RXNE    (1<<0)
#define SPI_TXE		(1<<1)

#define SPI_8BIT    (0<<11)
#define SPI_16BIT   (1<<11)

#define SPI_LSB     (1<<7)
#define SPI_MSB     (0<<7)

#define CPOL_HIGH   (1<<1)
#define CPHA_SECOND	(1<<0)

#define SPE         (1<<6)
#define SPI_BIDI    (1<<15)

#define SPI_MSTR    (1<<2)
#define SPI_SSI     (1<<8)
#define SPI_SSM     (1<<9)

#define LIS302_REG_CTRL_REG4       0x20
#define LIS302_REG_CTRL_REG6       0x25
#define LIS302_REG_CTRL_REG3       0x22
#define LIS302_REG_OUT_X   		   0x29
#define LIS302_REG_OUT_Y           0x2B
#define LIS302_REG_OUT_Z           0x2D
uint16_t read_buf[3]={0};

void tim_init();
void spi_init();
uint16_t spi_read(uint32_t reg);
void spi_write(uint8_t reg, uint8_t data);
void gpio_init();
void tim_oc_init();
void tim_IC_INIT();
char read(void);
static void UART_Rx_TX_Init(void);
static void USART_WRITE(uint8_t ch);
void ADC_init_TIMER_TRIGER();
void pwm_init();
void adc_start();
uint32_t adc_get_data();








#endif /* MAIN_H_ */
