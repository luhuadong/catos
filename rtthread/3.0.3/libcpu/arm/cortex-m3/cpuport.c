
#include "rtdef.h"

struct exception_stack_frame
{
	rt_uint32_t r0;
	rt_uint32_t r1;
	rt_uint32_t r2;
	rt_uint32_t r3;
	rt_uint32_t r12;
	rt_uint32_t lr;
	rt_uint32_t pc;
	rt_uint32_t psr;
};

struct stack_frame
{
	rt_uint32_t r4;
	rt_uint32_t r5;
	rt_uint32_t r6;
	rt_uint32_t r7;
	rt_uint32_t r8;
	rt_uint32_t r9;
	rt_uint32_t r10;
	rt_uint32_t r11;
	
	struct exception_stack_frame exception_stack_frame;
};

/**
 * This function will initialize thread stack
 * 
 * @param tentry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_addr the beginning stack address
 * 
 * @return stack address
 */
rt_uint8_t *rt_hw_stack_init(void       *tentry, 
	                           void       *parameter, 
												     rt_uint8_t *stack_addr)
{
	struct stack_frame *stack_frame;
	rt_uint8_t *stk;
	unsigned long i;
	
	/* get the top pointer of stack 	*/
	stk  = stack_addr + sizeof(rt_uint32_t);
	
	/* let stk pointer aligned down with 8-bytes */
	stk  = (rt_uint8_t *)RT_ALIGN_DOWN((rt_uint32_t)stk, 8);
	
	/* move down sizeof(struct stack_frame) offset */
	stk -= sizeof(struct stack_frame);
	stack_frame = (struct stack_frame *)stk;
	
	for(i=0; i<sizeof(struct stack_frame)/sizeof(rt_uint32_t); i++)
	{
		((rt_uint32_t *)stack_frame)[i] = 0xdeadbeef;
	}
	
	stack_frame->exception_stack_frame.r0  = (unsigned long)parameter; /* r0: argument */
	stack_frame->exception_stack_frame.r1  = 0;                        /* r1 */
	stack_frame->exception_stack_frame.r2  = 0;                        /* r2 */
	stack_frame->exception_stack_frame.r3  = 0;                        /* r3 */
	stack_frame->exception_stack_frame.r12 = 0;                        /* r12 */
	stack_frame->exception_stack_frame.lr  = 0;                        /* lr: initialized to 0 temporarily */
	stack_frame->exception_stack_frame.pc  = (unsigned long)tentry;    /* entry point, pc */
	stack_frame->exception_stack_frame.psr = 0x01000000L;              /* PSR */
	
	return stk;
}
