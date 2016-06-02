#include <xc.h>
#include "sched.h"
#include "fifo.h"

#define N_FIFOS         50

struct fifonode {
	struct fifonode *next;
	struct fifonode *prev;
	void *value;
};

static int fifo_sem;
static struct fifonode fifopool[N_FIFOS];
static struct fifonode *freelist;


static struct fifonode *fifonode_alloc()
{	
	struct fifonode *n = 0;

	sem_down(&fifo_sem);

	if (!freelist)
		goto out;

	n = freelist;	
	freelist = n->next;
	n->next = 0;
out:
	sem_up(&fifo_sem);

	return n;
}

static void fifonode_free(struct fifonode *n)
{
	sem_down(&fifo_sem);

	n->next = freelist;
	freelist = n;
	
	sem_up(&fifo_sem);
}

void fifo_init()
{
	int i;
	freelist = &fifopool[0];
	
	for (i=1;i<N_FIFOS;i++)
		fifopool[i-1].next = &fifopool[i];
	fifopool[N_FIFOS-1].next = 0;
	
	fifo_sem = 1;
}
		
int fifo_put(fifo_t *f, void *v)
{
	struct fifonode *n;
	int r = 0;

	sem_down(&f->sem);

	n = fifonode_alloc();
	if (!n)
		goto out;

	r = 1;
	n->value = v;
	n->next = 0;
	n->prev = f->tail;

	if (!f->head)
		f->head = n;

	f->tail = n;

out:
	sem_up(&f->sem);
	return r;
}

int fifo_get(fifo_t *f, void **v)
{
	struct fifonode *n;
	int r = 0;
	
	sem_down(&f->sem);

	n = f->head;
	if (!n)
		goto out;
	
	r = 1;
	f->head = n->next;

	if (n == f->tail)
		f->tail = 0;

	if (v)
		*v = n->value;
	
	fifonode_free(n);
out:
	sem_up(&f->sem);
	return r;
}
