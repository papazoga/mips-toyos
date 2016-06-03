/* 
 * Interrupt handling
 */

#include <xc.h>
#include <sys/attribs.h>
#include <stdint.h>
#include "regs.h"

struct interrupt_regs {
	vcsi_register_t control;
	vcsi_register_t shadow_priority;
	vcsi_register_t status;
	vcsi_register_t timer;
	vcsi_register_t flag[8];
	vcsi_register_t enable[8];
	vcsi_register_t priority[64];
	uint32_t offset[256];
};

#define INTERRUPT_BASE   0xbf810000

#define MVEC               (1<<12)
#define TPC_SHIFT          8
#define TPC_MASK           (0x7 << TPC_SHIFT)
#define EPOL_SHIFT         0
#define EPOL_MASK          (0x1f << EPOL_SHIFT)


__attribute__ ((address(INTERRUPT_BASE))) static volatile struct interrupt_regs ireg;

inline __attribute__((always_inline)) void int_clear_flag(int vector)
{
	ireg.flag[vector / 32].clr = (1 << (vector % 32));	
}

inline int int_get_flag(int vector)
{
	if (ireg.flag[vector / 32].val & (1 << (vector % 32)))
		return 1;
	return 0;
}

inline void int_set_flag(int vector)
{
	ireg.flag[vector / 32].set = (1 << (vector % 32));	
}

inline void int_set_priority(int vector, uint8_t pri, uint8_t spri)
{
	uint32_t pbits = (pri << 2) | spri;
	
	ireg.priority[vector / 4].clr = (0xff << ((vector % 4) * 8));
	ireg.priority[vector / 4].set = (pbits << ((vector % 4) * 8));
}

inline void int_enable(int vector)
{
	ireg.enable[vector / 32].set = (1 << (vector % 32));	
}

inline void int_disable(int vector)
{
	ireg.enable[vector / 32].clr = (1 << (vector % 32));	
}

void int_init()
{
	ireg.control.set = MVEC;
	ireg.shadow_priority.val = 0x65432100;
}
