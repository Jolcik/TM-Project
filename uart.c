#include <msp430.h>
#include <stdio.h>

#include "uart.h"

static int TRANSMIT_MESSAGE_BUF;

void init_uart(void (*f)(int)) {
    select_clock_signals();          // Assigns microcontroller clock signals
    assign_pins_to_uart();           // P4.2 is for TXD,  P4.3 is for RXD
    use_9600_baud();                 // UART operates at 9600 bits/second

    UCA3IE  = UCRXIE;                // Enable UART RXD interrupt
    RECEIVE_FUNCTION_POINTER = f;
}

void transmit_message(int message) {
    UCA3TXBUF = message;                // Send the UART message 0x56out pin P4.2
}

void select_clock_signals(void) {
    CSCTL0   = 0xA500;     // "Password" to access clock calibration registers
    CSCTL1   = 0x0046;     // Specifies frequency of main clock
    CSCTL2   = 0x0133;     // Assigns additional clock signals
    CSCTL3   = 0x0000;     // Use clocks at intended frequency, do not slow them down
}


void assign_pins_to_uart(void) {
    P6SEL1   = 0x00;
    P6SEL0   = BIT1 | BIT0;
}

void use_9600_baud(void) {
    UCA3CTLW0 = UCSWRST;                     // Put UART into SoftWare ReSeT
    UCA3CTLW0 = UCA3CTLW0 | UART_CLK_SEL;    // Specifies clock sourse for UART
    UCA3BR0   = BR0_FOR_9600;                // Specifies bit rate (baud) of 9600
    UCA3BR1   = BR1_FOR_9600;                // Specifies bit rate (baud) of 9600
    UCA3MCTLW = CLK_MOD;                     // "Cleans" clock signal
    UCA3CTLW0 = UCA3CTLW0 & (~UCSWRST);      // Takes UART out of SoftWare ReSeT}
}

#pragma vector=USCI_A3_VECTOR
__interrupt void UART_ISR(void)
{
    printf("%d \n", UCA3RXBUF);

    if (UART_MODE == RECEIVE) {
        static int previous_message;
        if (previous_message != NULL) {
            int message = (previous_message << 8) + UCA3RXBUF;
            (*RECEIVE_FUNCTION_POINTER)(message);
            previous_message = NULL;
        }
        else previous_message = UCA3RXBUF;
    }
    else if (UART_MODE == TRANSMIT && UCA3RXBUF == 0x00) {
        if (TRANSMIT_MESSAGE_BUF != NULL)
            UCA3TXBUF = TRANSMIT_MESSAGE_BUF;
        else
            printf("DUPA \n");
    }

    if(UCA3RXBUF == 0x56) {            // Check to see if the message is 0x56
        P1OUT = BIT0;// Turn on the redLED
        UCA3TXBUF = 0x00;
    }
    else if (UCA3RXBUF == 0x00) {
        P1OUT = 0x00;
        UCA3TXBUF = 0x72;
    }
    else if (UCA3RXBUF == 0x72) {
        P1OUT = BIT0;
        UCA3TXBUF = 0x00;
    }

//    if (UART_MODE == RECEIVE) {
//        printf("CHUJ \n");
//    }

    UCA3IFG = UCA3IFG & (~UCRXIFG); // Clear RX Interrupt FlaG
}
