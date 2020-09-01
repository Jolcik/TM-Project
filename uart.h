/*
 * uart.h
 *
 *  Created on: Aug 2020
 *  Author: Piotr Kostrzeñski
 */

#ifndef UART_H_
#define UART_H_


#define UART_CLK_SEL    0x0080    // Specifies accurate clock for UART peripheral
#define BR0_FOR_9600    0x34      // Value required to use 9600 baud
#define BR1_FOR_9600    0x00      // Value required to use 9600 baud
#define CLK_MOD         0x4911    // Microcontroller will "clean-up" clock signal

#define TRANSMIT 0x01
#define RECEIVE 0x02
unsigned static short int UART_MODE = RECEIVE;
void (*RECEIVE_FUNCTION_POINTER)(int);

void init_uart();
void transmit_message();
void select_clock_signals(void);     // Assigns microcontroller clock signals
void assign_pins_to_uart(void);      // P4.2 is for TXD,  P4.3 is for RXD
void use_9600_baud(void);            // UART operates at 9600 bits/second

#endif /* UART_H_ */
