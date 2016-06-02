#ifndef TIMER_H
#define TIMER_H

inline void timer_set_period(int, uint32_t);
inline uint32_t timer_period(int);
inline void timer_set_value(int, uint32_t);
inline uint32_t timer_value(int);
inline void timer_stop(int);
inline void timer_start(int);
void timer_prescale(int, uint8_t);

#endif
