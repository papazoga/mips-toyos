#ifndef _FIFO_H
#define _FIFO_H

typedef struct fifo fifo_t;

struct fifo {
	struct fifonode *head;
	struct fifonode *tail;
	int sem;
};

void fifo_init();
int fifo_put(fifo_t *f, void *v);
int fifo_get(fifo_t *f, void **v);

#endif
