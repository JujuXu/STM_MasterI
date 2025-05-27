#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stdbool.h>

void encoder_init(void);
void encoder_update(void);
void encoder_button_update(void);

int32_t encoder_get_ticks(void);
int16_t encoder_get_steps(void);
bool encoder_get_button(void);

void encoder_set_min(int16_t min_val);
void encoder_set_max(int16_t max_val);
void encoder_reset_position(void);

void encoder_set_cyclic(bool enable);
void encoder_set_direction(bool reverse);  // <-- nouveau

#endif
