#ifndef INTERRUPT_H
#define INTERRUPT_H

inline void int_disable(int);
inline void int_enable(int);
inline void int_set_priority(int, uint8_t, uint8_t);
inline void int_set_flag(int);
inline int int_get_flag(int);
inline void int_clear_flag(int);
void int_init();

#endif
