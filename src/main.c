// Pearl CV - Zack Nelson
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "midi.h"
#include "MAX528.h"

#define F_CPU 16000000UL

//Values per voice
//Pitch, Bend, Osc/Noise Mix, Over Tone, Attack, Filter, Decay, Level
uint8_t cvs[5][8] = {
        { 128, 96,  128, 128, 128, 128, 64,  128 },
        { 128, 128, 128, 64,  64,  128, 64,  128 },
        { 196, 128, 255, 128, 64,  128, 128, 128 },
        { 128, 128, 255, 128, 64,  128, 128, 128 },
        { 64,  128, 255, 128, 64,  128, 128, 128 }};

#define trig_max 57 //number of 5.7KHz cycles before trig turns off
uint8_t trig_cnt[5] = { 0, 0, 0, 0, 0 }; //time trigger has been high
uint8_t trig_velo[5] = { 127, 127, 127, 127, 127 }; //amplitude of trigger

uint8_t mux_cnt = 0;

ISR (INT0_vect) {
    mux_cnt = 0;
}

ISR (INT1_vect) { //write CV
    uint8_t x = mux_cnt & 0b111;
    uint8_t y = (mux_cnt >> 3) & 0b111;
    if (mux_cnt++ >= 40) mux_cnt = 0; //error catch
    uint8_t cv = cvs[y][x]; //lookup CV
    max_wr_command(0b1, cv); //write CV to out0

    for (int voice = 0; voice < 5; voice++) {
        if (trig_cnt[voice]) { //if trig active
            if (trig_cnt[voice] == 1) { //turn trig on
                max_wr_command(0b1000 << voice, trig_velo[voice]); //write CV
            } else if (trig_cnt[voice] >= trig_max) { //trig turn off
                max_wr_command(0b1000 << voice, 0); //write CV
            }

            trig_cnt[voice]++;
        }
    }
}

void note_on(uint8_t note, uint8_t vel) {
    uint8_t voice = 0xff;
    switch (note) {
        case 48://35: //Bass
        case 49://36: //Kick
            voice = 0;
            break;
        case 50://38: //Snare
        case 51://40: //Electronic Snare
            voice = 1;
            break;
        case 52://43: //Floor tom
        case 53://45: //Low Tom
            voice = 2;
            break;
        case 54://47: //Low-Mid Tom
        case 55://48: //High-Mid Tom
            voice = 3;
            break;
        case 56://50: //High Tom
            voice = 4;
    }

    if (voice != 0xff) {
        trig_cnt[voice] = 1; //enable
        trig_velo[voice] = vel << 1; //set velocity
    }
}

void control_change(uint8_t param, uint8_t val) {
    //calculate position in voice value matrix
    uint8_t x = param & 0b111;
    uint8_t y = param >> 3;

    if (y >= 4) y = 4;
    cvs[y][x] = val << 1; //store value
}

int main() {
    uart_init(31250);

    set_cntrl_chng(control_change); //set function to handle control changes
    set_MIDI_key_press(note_on); //set function to handle note on
    //set_MIDI_key_release(); //set function to handle note off

    UCSR0B &= ~(1 << TXEN0); //Disable transmitter

    cli();// disable all interrupts

    //setup external interrupts
    EICRA |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00); //rising edge
    //MCUCR |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00); //rising edge
    EIMSK |= (1 << INT1) | (1 << INT0); //enable int
    //GIMSK |= (1 << INT1) | (1 << INT0);      // Turns on INT0

    DDRB = 0xff; //DAC SPI data bus
    //PB3 MOSI
    //PB4 MISO
    //PB5 SCK
    DDRC = 0b00111111; //switch outputs
    //PC1 CV Switch
    DDRD = 0b11110010; //0=input
    //PD0 = MIDI in
    //PD2 = INT0 = Count Reset
    //PD3 = INT1 = CV clock
    //PD4 = DAC CS
    PORTD = 0b1000; //pullup INT pin
    PORTB = 0;
    PORTC = 0; //switches off

    //-------------------------------
    max_init(); //configure DAC
    _delay_ms(1);

    sei();// enable all interrupts

    while (1) if (uart_test()) handle_midi();

    return 1;
}