#include <xc.h>
#include <stdint.h>

#include "sched.h"
#include "fifo.h"

void blinky1();
void blinky2();
uint32_t blinky1_stack[256];
uint32_t blinky2_stack[256];

task_t blinky1_task = {
	.name = "blinky1",
	.stackp = &blinky1_stack[256],
	.func = &blinky1
};


task_t blinky2_task = {
	.name = "blinky2",
	.stackp = &blinky2_stack[256],
	.func = &blinky2
};

int sem = 1;

fifo_t blinky_fifo = {
	.sem = 1,
};

void blinky1()
{
	uint32_t v[5] = { 1,3,2,5,7 };
	int i, j=0;
	
	while (1) {
		while (!fifo_put(&blinky_fifo, &v[j]))
			sched_yield();

		j = (j+1)%5;
		for (i=0;i<5000000;i++);
	}
}

void blinky2()
{
	uint32_t *v;
	
	while (1) {
		while (!fifo_get(&blinky_fifo, &v))
			sched_yield();

		LATH = *v;
	}
}
