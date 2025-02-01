//UART functions
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

typedef struct _fifo {
    uint8_t idx_w;
    uint8_t idx_r;
    uint8_t count;
    uint8_t buff[64];
} FIFO;

static volatile FIFO rxfifo;

//Initialize UART
void uart_init(long baud) {
    rxfifo.idx_r = 0;
    rxfifo.idx_w = 0;
    rxfifo.count = 0;

    UBRR0L = F_CPU / baud / 16 - 1;
    UCSR0B = _BV(RXEN0) | _BV(RXCIE0) | _BV(TXEN0);
}

//Test if there are bytes in the read buffer
uint8_t uart_test() {
    return rxfifo.count;
}

//Get a received character
uint8_t uart_get() {
    while (rxfifo.count == 0);

    uint8_t data = rxfifo.buff[rxfifo.idx_r++];
    cli();
    rxfifo.count--;
    sei();
    if (rxfifo.idx_r >= sizeof(rxfifo.buff)) rxfifo.idx_r = 0;

    return data;
}

//UART RXC interrupt
ISR(USART_RX_vect) {
    if (rxfifo.count < sizeof(rxfifo.buff)) {
        rxfifo.count++;
        rxfifo.buff[rxfifo.idx_w++] = UDR0;
        if (rxfifo.idx_w >= sizeof(rxfifo.buff)) rxfifo.idx_w = 0;
    }
}