//MAX528 DAC - Zack Nelson
#include <avr/io.h>
#include <util/delay.h>
#include "MAX528.h"

void max_init() {
    MAXDDR |= (1 << MAX_DAT) | (1 << MAX_CLK) | (1 << MAX_CS);
    DDRB = 0xff; //data out

    SPCR = (1 << SPE) | (1 << MSTR); //enable spi as master
    SPSR = (1 << SPI2X); //double speed clock
    max_wr_command(0x00, 0xff); //buffer all outs
}

void max_wr_byte(uint8_t out_byte) {
    SPDR = out_byte;
    while (!(SPSR & (1 << SPIF)));
}

void max_wr_command(uint8_t addr, uint8_t data) {
    MAXPORT &= ~(1 << MAX_CS); //enable cs
    SPDR = addr;
    while (!(SPSR & (1 << SPIF)));
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    MAXPORT |= (1 << MAX_CS); //disable cs
}