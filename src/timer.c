/* 
 * Timer Management
 */

#include <xc.h>
#include <stdint.h>
#include "regs.h"

#define N_TIMERS                9

/* Register map (128 DWORDS = 512B) */
struct timer_regs {
	__attribute__((aligned(512))) vcsi_register_t control;
	vcsi_register_t counter;
	vcsi_register_t period;	/* 0x20 */	
};

/* Control bits */
#define ON      (1<<15)
#define SIDL    (1<<13)
#define TGATE   (1<<7)
#define PRESCALE_SHIFT 4
#define PRESCALE_MASK  (7<<PRESCALE_SHIFT)
#define T32     (1<<3)
#define TCS     (1<<1)

/* Timer SFRs */
__attribute__ ((address(0xbf840000))) static volatile struct timer_regs timer[N_TIMERS];

void timer_prescale(int n, uint8_t prescale)
{
	uint32_t tmp = timer[n].control.val;

	tmp &= ~PRESCALE_MASK;
	prescale &= (PRESCALE_MASK >> PRESCALE_SHIFT);
	tmp |= (prescale << PRESCALE_SHIFT);

	timer[n].control.val = tmp;
}

inline void timer_start(int n)
{
	timer[n].control.set = ON;
}

inline void timer_stop(int n)
{
	timer[n].control.clr = ON;
}

inline uint32_t timer_value(int n)
{
	return timer[n].counter.val;
}

inline void timer_set_value(int n, uint32_t value)
{
	timer[n].counter.val = value;
}

inline uint32_t timer_period(int n)
{
	return timer[n].period.val;
}

inline void timer_set_period(int n, uint32_t period)
{
	timer[n].period.val = period;
}
