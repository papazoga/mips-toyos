#include <xc.h>
#include "sched.h"

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

void blinky1()
{
	int i;

	while (1) {
		for (i=0;i<2000000;i++);
		LATHINV = 1;
	}
}

void blinky2()
{
	int i;

	while (1) {
		for (i=0;i<5000000;i++);
		LATHINV = 2;
	}
}
