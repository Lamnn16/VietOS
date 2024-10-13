#include "stm32f4xx.h"  // Include device-specific header

void delay_ms(uint32_t delay);

int main(void)
{
    // 1. Enable the clock for GPIOA (connected to the LED on PA5)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Set PA5 as output
    GPIOA->MODER &= ~(GPIO_MODER_MODE5);     // Clear mode for PA5
    GPIOA->MODER |= (1 << GPIO_MODER_MODE5_Pos);  // Set PA5 as output (01)

    // 3. Configure PA5 as push-pull output (default)
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);    // Clear output type (push-pull)
    
    // 4. Configure PA5 for no pull-up/pull-down resistors
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5);    // No pull-up, no pull-down
    
    while(1)
    {
        // 5. Toggle the LED on PA5
        GPIOA->ODR ^= GPIO_ODR_OD5;  // Toggle PA5
        
        // 6. Add a delay
        delay_ms(500);  // 500ms delay
    }
}

void delay_ms(uint32_t delay)
{
    // Simple delay loop (not accurate for real-time use)
    volatile uint32_t i;
    for(i = 0; i < delay * 16000; i++);  // Assuming a 16 MHz clock
}