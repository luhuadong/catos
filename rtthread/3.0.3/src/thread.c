
#include "rtdef.h"
#include "rtservice.h"

/**
 * This function will initialize thread
 * 
 * @param thread the control block of thread
 * @param entry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_start the beginning stack address
 * @param stack_size the size of stack
 * 
 * @return RT_EOK if not error
 */
rt_err_t rt_thread_init(struct rt_thread *thread,
                        void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												rt_uint32_t stack_size)
{
	rt_list_init(&(thread->tlist));
	
	thread->entry = (void *)entry;
	thread->parameter = parameter;
	
	thread->stack_addr = stack_start;
	thread->stack_size = stack_size;
	
	thread->sp = 
	(void *)rt_hw_stack_init(thread->entry,
	                         thread->parameter,
	                         (void *)((char *)thread->stack_addr + thread->stack_size - 4));
	
	return RT_EOK;
}
