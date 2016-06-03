/*
 * A UART driver
 */
#include <xc.h>
#include <sys/attribs.h>
#include <stdint.h>
#include "regs.h"
#include "interrupt.h"

struct uart_regs {
	vcsi_register_t mode;
	vcsi_register_t status;
	uint32_t tx;
	uint32_t _blank1[3];
	uint32_t rx;
	uint32_t _blank2[3];
	vcsi_register_t baudrate;
};

#define UART1_REG_BASE ((void *)0xbf822000)
#define UART2_REG_BASE ((void *)0xbf822200)
#define UART3_REG_BASE ((void *)0xbf822400)
#define UART4_REG_BASE ((void *)0xbf822600)
#define UART5_REG_BASE ((void *)0xbf822800)
#define UART6_REG_BASE ((void *)0xbf822a00)

#define UART1_IRQ_BASE  _UART1_FAULT_VECTOR
#define UART2_IRQ_BASE  _UART2_FAULT_VECTOR
#define UART3_IRQ_BASE  _UART3_FAULT_VECTOR
#define UART4_IRQ_BASE  _UART4_FAULT_VECTOR
#define UART5_IRQ_BASE  _UART5_FAULT_VECTOR
#define UART6_IRQ_BASE  _UART6_FAULT_VECTOR

struct uart_s {
	uint32_t irqbase;
	volatile struct uart_regs *const regs;
} uart[] = {
	{ 0, 0},		/* empty */
	{ UART1_IRQ_BASE, UART1_REG_BASE },
	{ UART2_IRQ_BASE, UART2_REG_BASE },
	{ UART3_IRQ_BASE, UART3_REG_BASE },
	{ UART4_IRQ_BASE, UART4_REG_BASE },
	{ UART5_IRQ_BASE, UART5_REG_BASE },
	{ UART6_IRQ_BASE, UART6_REG_BASE },
};

#define FAULT_IRQ(x)       (x.irqbase)
#define RX_IRQ(x)          (x.irqbase+1)
#define TX_IRQ(x)          (x.irqbase+2)

#define BUFSIZE               1024

#define ON                    (1<<15)
#define BRGH                  (1<<3)

#define UTXISEL_SHIFT         14
#define UTXISEL_MASK          (3 << UTXISEL_SHIFT)
#define UTXEN                 (1<<10)
#define UTXBF                 (1<<9)
#define TRMT                  (1<<8)

struct cbuf {
	char data[BUFSIZE];
	int head;
	int tail;	
} txbuf;

#define main_uart        (uart[2])

int uart_init(unsigned int baud)
{	
	int_set_priority(FAULT_IRQ(main_uart), 2, 0);
	int_set_priority(RX_IRQ(main_uart), 2, 0);
	int_set_priority(TX_IRQ(main_uart), 2, 0);
	int_clear_flag(RX_IRQ(main_uart));
	int_clear_flag(TX_IRQ(main_uart));

	main_uart.regs->mode.val = ON | BRGH;
	main_uart.regs->baudrate.val = 216;	       /* 115200bps */
	
	main_uart.regs->status.val = (1<<UTXISEL_SHIFT) | UTXEN;
	
	txbuf.head = 0;
	txbuf.tail = 0;
}

static int txpop(uint32_t *dst)
{
	if (txbuf.head == txbuf.tail)
		return 0;
	*dst = txbuf.data[txbuf.head];
	txbuf.head = (txbuf.head + 1) % BUFSIZE;
	return 1;
}

void __ISR(_UART2_FAULT_VECTOR, IPL2SRS) uart2_fault_isr()
{
	int_clear_flag(_UART2_FAULT_VECTOR);
}

void __ISR(_UART2_RX_VECTOR, IPL2SRS) uart2_rx_isr()
{
	int_clear_flag(_UART2_RX_VECTOR);
}

void __ISR(_UART2_TX_VECTOR, IPL2SRS) uart2_tx_isr()
{	

	if (!txpop(&main_uart.regs->tx))
		int_disable(_UART2_TX_VECTOR);

	int_clear_flag(TX_IRQ(main_uart));
	return;
}

int uart_tx(const char *data, int len)
{
	int count;

	if (len == 0)
		return 0;
	
	for (count=0;count<len;count++) {
		int new_tail = (txbuf.tail + 1) % BUFSIZE;
		if (new_tail == txbuf.head)
			break;
		txbuf.data[txbuf.tail] = data[count];
		txbuf.tail = new_tail;
	}

	int_enable(_UART2_TX_VECTOR);
	txpop(&main_uart.regs->tx);

	return count;
}

int uart_rx(unsigned char *x)
{
}

