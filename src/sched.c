#include <xc.h>
#include "sched.h"
#include "interrupt.h"
#include "timer.h"

task_t *current_task;
taskq_t ready_queue;
taskq_t *readyq = &ready_queue;

#define SCHED_TIMER                 1 /* use TIMER2 */
#define SCHED_TIMER_IRQ             (_TIMER_1_VECTOR + 5*SCHED_TIMER)
#define SCHED_TIMER_PRESC           0x7
#define SCHED_TIMER_PERIOD          0x2000
#define SCHED_TIMER_PRIORITY        1

static task_t idle = {
	.name = "idle",
	.state = TASK_STOPPED,
	.next = 0,
	.prev = 0
};

static void idle_func()
{
	timer_start(SCHED_TIMER);
	int_enable(SCHED_TIMER_IRQ);

	for (;;);
}

void taskq_init(taskq_t *q)
{
	q->head = 0;
	q->tail = 0;
}

void taskq_append(taskq_t *q, task_t *t)
{
	t->next = 0;
	t->prev = q->tail;
	t->queue = q;

	if (q->head == 0)
		q->head = t;
	else
		q->tail->next = t;
	
	q->tail = t;
}


void taskq_remove(taskq_t *q, task_t *t)
{
	if (q != t->queue)
		return;

	t->queue = 0;

	if (q->head == t) {
		q->head = t->next;
		if (q->tail == t)
			q->tail = 0;
		else
			t->next->prev = 0;
	} else {
		t->prev->next = t->next;
		if (q->tail == t)
			q->tail = t->prev;
		else
			t->next->prev = t->prev;
	}
}

task_t *taskq_pop(taskq_t *q)
{
	task_t *t;
	
	if (!q->head)
		return 0;

	t = q->head;
	taskq_remove(q, q->head);

	return t;
}

void sched_init()
{
	taskq_init(readyq);
	taskq_append(readyq, &idle);
	current_task = &idle;

	idle.state = TASK_READY;	

	/* The idle tasks inherits the system stack, so
	   no stack setup is required. */

	timer_stop(SCHED_TIMER);
	timer_set_value(SCHED_TIMER, 0);
	timer_prescale(SCHED_TIMER, SCHED_TIMER_PRESC);
	timer_set_period(SCHED_TIMER, SCHED_TIMER_PERIOD);

	int_clear_flag(SCHED_TIMER_IRQ);
	int_set_priority(SCHED_TIMER_IRQ, SCHED_TIMER_PRIORITY, 0);

	int_clear_flag(_CORE_SOFTWARE_0_VECTOR);
	int_set_priority(_CORE_SOFTWARE_0_VECTOR, SCHED_TIMER_PRIORITY, 0);
}

void sched_start()
{
	/* The idle function actually starts the timer, and never returns */
	idle_func();

	/* This should never execute. */
	for (;;);
}

void sched_spawn(task_t *t)
{
	register uint32_t general_pointer asm("$28");

	/* Make sure the stack pointer is properly aligned */
	t->stackp = (void *)(((uint32_t)t->stackp) & (~0xf));

	/* Make the stack frame ready */
	t->stackp -= 40;
	t->stackp[3] = 0x25000003;	  /* CP0_STATUS */
	t->stackp[4] = (uint32_t)t->func; /* CP0_EPC */
	t->stackp[8] = general_pointer;

	/* Add to ready queue */
	taskq_append(readyq, t);
	t->state = TASK_READY;
}

void sched_yield()
{
	uint32_t cause;

	cause = _mfc0(13, 0);
	cause |= (1<<8);
	_mtc0(13, 0, cause);
}

/* Called from the ISR. Must clear the interupt flag. */
void schedule()
{
	current_task = current_task->next;

	if (!current_task)
		current_task = readyq->head;
	
	int_clear_flag(SCHED_TIMER_IRQ);
	int_clear_flag(_CORE_SOFTWARE_0_VECTOR);
}

int sem_up(int *sem)
{
	int t;

	__builtin_disable_interrupts();
	t = *sem;
	t++;
	*sem = t;
	__builtin_enable_interrupts();

	return t;
}

int sem_down(int *sem)
{
	int t;

	do {
		__builtin_disable_interrupts();
		t = *sem;
		if (t > 0)
			*sem = t-1;
		__builtin_enable_interrupts();
	
		if (t <= 0)
			sched_yield();

	} while(t <= 0);

	return t-1;
}
