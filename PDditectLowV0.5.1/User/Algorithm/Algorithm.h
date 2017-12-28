#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "stm32f4xx.h"

void get_envelope_up (u16 envelope_up[],u16 size_up);
void get_envelope_below (u16 envelope_below[],u16 size_below);
void get_rectangle_up(u16 rectangleXY[] ,u16 Rctangle_len);
void get_rectangle_below(u16 rectangleXY[] ,u16 Rctangle_len);
void get_interval_up(u16 intervalXY[] ,u16 Interval_len);
void get_interval_below(u16 intervalXY[] ,u16 Interval_len);
void debug_out (void);
void compare_out(void);

#endif
