# Pearl Drum-X MIDI retrofit
 A MIDI retrofit board for Pearl's Drum-X (DRX-1) drum synthesizer


The project is designed around an Atmega*8 that controls a MAX528 octal DAC.

### DAC
Five DAC outputs are used to generate triggers with different voltages based on MIDI velocity.

One output is used to generate eight control voltages for each of the five voices. Timing signals are received from the Pearl's motherboard, and are used to syncronize the stream of multiplexed CV signals.

### Use
The boards starts up in "MIDI learn" mode, and loads default settings for each voice. The board will assign itself to the first channel it receives a message on. The notes that it responds to, and the voice defaults are hard-coded in the firmware.

### Atmega328p
The fuse bits of the Atmega328p need to be set for an external 16MHz crystal oscillator. The chips default to using the internal osc, and the firmware does not support this.
The following values can be programmed with avrdude: L:0xFF, H:0xD9, E:0xFF 

### Schematic
![Schematic](/sch/Pearl%20MIDI%20schematic.png)

![Board](/sch/Pearl%20MIDI%20board.png)
