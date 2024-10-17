#include <stdio.h>
#include "led.h"  // Include device-specific header
#include "uart.h"


int main(void)
{	

    led_init();
		uart_tx_init();
	
    while(1)
    {
        printf("Hello from Vietnam.......\n\r");
    }
}

