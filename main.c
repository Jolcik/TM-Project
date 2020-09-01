#include <msp430.h>
#include <stdio.h>

#include "uart.h"

#define ENABLE_PINS     0xFFFE    // Required to use inputs and outputs

void say_hello(int message) {
    printf("CHUJ %d\n", message);
}

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;       // Stop WDT
    PM5CTL0 = ENABLE_PINS;           // Enable pins
    P1DIR   = BIT0;                  // Make P1.0 an output for red LED
    P1OUT   = 0x00;                  // Red LED initially off


    init_uart(&say_hello);
    UART_MODE = RECEIVE;
    _BIS_SR(GIE);                    // Activate enabled UART RXD interrupt


    transmit_message(0x56);


    while(1);                        // Wait here unless you get UART interrupt}

}
