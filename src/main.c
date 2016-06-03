#include <xc.h>
#include "sched.h"
#include "config.h"
#include "interrupt.h"
#include "fifo.h"

extern task_t blinky1_task;	/* Defined in blinky.c */
extern task_t blinky2_task;	/* Defined in blinky.c */

static void sys_unlock()
{
	SYSKEY = 0xaa996655;
	SYSKEY = 0x556699aa;
}

static void sys_lock()
{
	SYSKEY = 0x33333333;
}

static int sys_init()
{
	/* PPS Configuration */
	sys_unlock();

	CFGCON &= ~(1<<13);
	RPB14R = 0x02;		/* RPB14 to U2TX */

	sys_lock();

	ANSELHCLR = 7;
	TRISHCLR = 7;
}

int main()
{
	/* Take care of pin configuration */
	sys_init();

	/* Initialize the interrupt controller */
	int_init();
	__builtin_enable_interrupts();

	/* UART */
	uart_init();

	/* Initialize the scheduler */
	sched_init();

	/* Initialize the fifo pool */
	fifo_init();
	
	/* Add the blinky tasks */
	sched_spawn(&blinky1_task);
	sched_spawn(&blinky2_task);

	/* Start the scheduler. This call never returns. */
	sched_start();

	/* Keep GCC happy. */
	for (;;);
}

void _general_exception_handler(unsigned int cause, unsigned int status)
{
	LATHSET = 7;
	for (;;);
}
