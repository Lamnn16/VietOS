#include <stdio.h>
#include "uart.h"
#include "stm32f411xe.h"


#define GPIOAEN 			(1U<<0U)
#define UART2EN 			(1U<<17)
#define SYS_FREQ 			16000000   /* By default */
#define APB1_CLK			SYS_FREQ
#define BAUDRATE			115200
#define CR1_UE				(1U<<13)
#define CR1_TE				(1U<<3)
#define SR_TXE				(1U<<7)


#define compute_uart_bd(periph_clk, baudrate) (((periph_clk + (baudrate/2U))/baudrate))


static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart_write(int ch);


struct ___FILE{int handle;};
FILE   __stdout;
FILE   __stdin;

int fputc(int ch, FILE *f)
{
	uart_write(ch);
	return ch;
}


void uart_tx_init(void) {
	/* Enable clock access to GPIOA */
	RCC -> AHB1ENR |= GPIOAEN;
	
	/* Set PA2 mode to alternate function mode */
	GPIOA->MODER &=~ (1U<<4);
	GPIOA->MODER |= (1U<<5);
	
	/* Set alternate function type to AF7 (UART_Tx) */
	/* There is array with two elements representing for AFRL anf AFRH */
	GPIOA->AFR[0] &=~ (1U<<8)|(1U<<9)|(1U<<10)|(1U<<11);
	GPIOA->AFR[0] |= (1U<<8)|(1U<<9)|(1U<<10);
	
	/* Enable clock access to UART */
	RCC->APB1ENR |= UART2EN;
	
	/* Configure baudrate */
	uart_set_baudrate(APB1_CLK, BAUDRATE);
	
	/* Configure transfer direction */
	USART2->CR1 = CR1_TE; 

	/* Enable UART mode */
	USART2->CR1 |= CR1_UE;
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate) 
{
	USART2->BRR = compute_uart_bd(periph_clk, baudrate); 
}


static void uart_write(int ch)
{
	/* Make sure the transmit data register is empty */
	while(!(USART2->SR & SR_TXE)){}
	/* Write to transmit data register */
	USART2->DR = (ch & 0xFF);
}
