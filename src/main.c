#include <xc.h>
#include "sched.h"
#include "config.h"
#include "interrupt.h"
#include "fifo.h"

extern task_t blinky1_task;	/* Defined in blinky.c */
extern task_t blinky2_task;	/* Defined in blinky.c */

int main()
{
	ANSELHCLR = 7;
	TRISHCLR = 7;

	/* Initialize the interrupt controller */
	int_init();
	__builtin_enable_interrupts();
	
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
