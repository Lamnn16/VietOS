#include <stdio.h>
#include "led.h"  // Include device-specific header
#include "uart.h"
#include "timebase.h"

int main(void)
{	

    led_init();
		uart_tx_init();
		timebase_init();
	
    while(1)
    {
        led_on();
			  delay(1);
			  led_off();
			  delay(1);
    }
}

