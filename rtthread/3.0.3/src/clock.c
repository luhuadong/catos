#include <rtthread.h>
#include <rthw.h>

static rt_tick_t rt_tick = 0;

rt_tick_t rt_tick_get(void)
{
	return rt_tick;
}
	
void rt_tick_increase(void)
{
#if 0
	rt_ubase_t i;
	struct rt_thread *thread;
	rt_tick++;
	
	for(i=0; i<RT_THREAD_PRIORITY_MAX; i++)
	{
		thread = rt_list_entry(rt_thread_priority_table[i].next, struct rt_thread, tlist);
		
		if(thread->remaining_tick > 0)
		{
			thread->remaining_tick--;
			
			if(thread->remaining_tick == 0)
			{
				//rt_schedule_insert_thread(thread);
				rt_thread_ready_priority_group |= thread->number_mask;
			}
		}
	}
	
	rt_schedule();
#else
	++rt_tick;
	rt_timer_check();
#endif
}
