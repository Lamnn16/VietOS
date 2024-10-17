#include "led.h"

#define GPIOA_EN 		(1U<<0)
#define LED_PIN 		(1U<<5)

void led_init(void)
{
	/* Enable clock access to led port - PORTA */
	RCC->AHB1ENR |= GPIOA_EN;
	
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);
}

void led_on(void)
{
  /* Set PA5 */
	GPIOA->ODR  |= LED_PIN;
}

void led_off(void){
	GPIOA->ODR  &= ~LED_PIN;
}
	