#ifndef _SCHED_H
#define _SCHED_H

#include <stdint.h>

#define TASK_NAME_LENGTH            8

typedef struct task task_t;
typedef struct taskq taskq_t;

#define TASK_STOPPED                0
#define TASK_READY                  1
#define TASK_BLOCKED                2

struct task {
	uint32_t *stackp;	/* must be the first member of the struct */
	char name[TASK_NAME_LENGTH];
	task_t *next, *prev;
	taskq_t *queue;
	int state;
	void (*func)();
};

struct taskq {
	struct task *head;
	struct task *tail;
};

/* Initialize the ready queue and set idle task running.
   Called from main(). */
void sched_init();

/* Start a task. The task descriptor must be pre-allocated and initialized. */
void sched_spawn(task_t *);

/* Start the idle task and the scheduler. */
void sched_start();

/* Schedule the next task */
void schedule();

/* Task queue management */
void taskq_init(taskq_t *);
void taskq_append(taskq_t *, task_t *);
void taskq_remove(taskq_t *, task_t *);

#endif
