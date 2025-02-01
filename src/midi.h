
#include <stdint.h>
#ifndef MIDI_H_
#define MIDI_H_
uint8_t handle_com(uint8_t com, uint8_t arg_cnt, uint8_t *args);
uint8_t handle_glob_com(uint8_t com, uint8_t arg_cnt, uint8_t *args);
uint8_t skip_com(uint8_t com, uint8_t arg_cnt, uint8_t *args);
void handle_midi(void);
void handle_realtime(uint8_t);
void set_MIDI_key_press(void (*ptr)(uint8_t, uint8_t));
void set_MIDI_key_release(void (*ptr)(uint8_t));
void set_song_pos_ptr(void (*ptr)(int));
void set_rt_clock(void (*ptr)());
void set_rt_start(void (*ptr)());
void set_rt_cont(void (*ptr)());
void set_rt_stop(void (*ptr)());
void set_cntrl_chng(void (*ptr)(uint8_t,uint8_t));
void set_prg_chng(void (*ptr)(uint8_t));
void set_MIDI_pitch_bend(void (*ptr)(int));

void (*MIDI_key_press)(uint8_t, uint8_t);
void (*MIDI_key_release)(uint8_t);
void (*song_pos_ptr)(int);
void (*rt_clock)();
void (*rt_start)();
void (*rt_cont)();
void (*rt_stop)();
void (*cntrl_chng)(uint8_t, uint8_t);
void (*prg_chng)(uint8_t);
void (*MIDI_pitch_bend)(int);

#endif /* MIDI_H_ */
