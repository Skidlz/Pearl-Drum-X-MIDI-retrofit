//MAX528 DAC - Zack Nelson
#ifndef MAX528_H_
#define MAX528_H_

#include <stdint.h>

#define MAXDDR DDRD
#define MAXPORT PORTD
#define MAX_CLK 6
#define MAX_DAT 7
#define MAX_CS 4

void max_init();
void max_wr_byte(uint8_t);
void max_wr_command(uint8_t, uint8_t);

#endif /* MAX528_H_ */