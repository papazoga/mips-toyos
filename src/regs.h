#ifndef _REGS_H
#define _REGS_H

/* A register map with clear, set and invert */
struct vcsi_register {
	uint32_t val;
	uint32_t clr;
	uint32_t set;
	uint32_t inv;
};

typedef struct vcsi_register vcsi_register_t;

/* #define vcsi_register(NAME)          union {				\ */
/*        		uint32_t NAME;						\ */
/* 		struct {						\ */
/* 			uint32_t val;					\ */
/* 			uint32_t clr;					\ */
/* 			uint32_t set;					\ */
/* 			uint32_t inv;					\ */
/* 		} _##NAME;						\ */
/* 	} */

#endif
