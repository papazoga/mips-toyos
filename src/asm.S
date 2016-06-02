#include <xc.h>

	.set noat
	.set nomips16
	.globl __vector_dispatch_9
	.globl __vector_dispatch_1
	.extern schedule
	.extern current_task

	.text

scheduler_isr:	
	addiu sp,sp,-144	/* reserve space for stack frame */
	sw $1, 140(sp)
	sw $2, 136(sp)
	sw $3, 132(sp)
	sw $4, 128(sp)
	sw $5, 124(sp)
	sw $6, 120(sp)
	sw $7, 116(sp)
	sw $8, 112(sp)
	sw $9, 108(sp)
	sw $10, 104(sp)
	sw $11, 100(sp)
	sw $12, 96(sp)
	sw $13, 92(sp)
	sw $14, 88(sp)
	sw $15, 84(sp)
	sw $16, 80(sp)
	sw $17, 76(sp)
	sw $18, 72(sp)
	sw $19, 68(sp)
	sw $20, 64(sp)
	sw $21, 60(sp)
	sw $22, 56(sp)
	sw $23, 52(sp)
	sw $24, 48(sp)
	sw $25, 44(sp)
	sw $26, 40(sp)
	sw $27, 36(sp)
	sw $28, 32(sp)
        /* $29 is sp */
	sw $30, 28(sp)
	sw $31, 24(sp)

	/* Save EPC and STATUS */
	mfc0 k0, _CP0_EPC
	mfc0 k1, _CP0_STATUS
	sw k0, 16(sp)
	sw k1, 12(sp)
	ehb

	/* Put the stack pointer into the task_struct */
	la s7, current_task
	lw s7, (s7)
	sw sp, (s7)
	
	jal schedule
	nop

	/* Get the stack pointer from the current_task */
	la s7, current_task
	lw s7, (s7)
	lw sp, (s7)

	/* Restore EPC and STATUS */
	lw k1, 12(sp)
	lw k0, 16(sp)
	mtc0 k1, _CP0_STATUS
	mtc0 k0, _CP0_EPC

	lw $1, 140(sp)
	lw $2, 136(sp)
	lw $3, 132(sp)
	lw $4, 128(sp)
	lw $5, 124(sp)
	lw $6, 120(sp)
	lw $7, 116(sp)
	lw $8, 112(sp)
	lw $9, 108(sp)
	lw $10, 104(sp)
	lw $11, 100(sp)
	lw $12, 96(sp)
	lw $13, 92(sp)
	lw $14, 88(sp)
	lw $15, 84(sp)
	lw $16, 80(sp)
	lw $17, 76(sp)
	lw $18, 72(sp)
	lw $19, 68(sp)
	lw $20, 64(sp)
	lw $21, 60(sp)
	lw $22, 56(sp)
	lw $23, 52(sp)
	lw $24, 48(sp)
	lw $25, 44(sp)
	lw $26, 40(sp)
	lw $27, 36(sp)
	lw $28, 32(sp)
        /* $29 is sp */
	lw $30, 28(sp)
	lw $31, 24(sp)
	addiu sp,sp,144

	ehb
	eret
	nop

/* Core Software Interrupt 0 */
	.section .vector_1,code
	.globl __vector_dispatch_1
	.align 2
	.ent __vector_dispatch_1

__vector_dispatch_1:
	j scheduler_isr
	nop

	.end __vector_dispatch_1
	.size __vector_dispatch_1, .-__vector_dispatch_1

/* Timer 2 Interrupt */	
	.section .vector_9,code
	.globl __vector_dispatch_9
	.align 2
	.ent __vector_dispatch_9

__vector_dispatch_9:
	j scheduler_isr
	nop

	.end __vector_dispatch_9
	.size __vector_dispatch_9, .-__vector_dispatch_9

	
