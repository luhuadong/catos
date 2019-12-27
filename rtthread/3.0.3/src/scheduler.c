
#include <rtthread.h>
#include <rthw.h>

/* thread ready priority group, only support 32 level now */
rt_uint32_t rt_thread_ready_priority_group;

/* thread ready list */
rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

struct rt_thread *rt_current_thread;
rt_uint8_t rt_current_priority;
				
extern struct rt_thread idle;
extern struct rt_thread rt_flag1_thread;
extern struct rt_thread rt_flag2_thread;

/**
 * This function will initialize scheduler
 */
void rt_system_scheduler_init(void)
{
	register rt_base_t offset;
	
	for(offset=0; offset<RT_THREAD_PRIORITY_MAX; offset++)
	{
		rt_list_init(&rt_thread_priority_table[offset]);
	}
	
	rt_current_priority = RT_THREAD_PRIORITY_MAX - 1;
	rt_current_thread = RT_NULL;
	
	rt_thread_ready_priority_group = 0;
}

/**
 * This function will start scheduler
 */
void rt_system_scheduler_start(void)
{
#if 0
	register struct rt_thread *to_thread;
	
	to_thread = rt_list_entry(rt_thread_priority_table[0].next,
	                          struct rt_thread,
														tlist);
	
	rt_current_thread = to_thread;
	rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
#else
	register struct rt_thread *to_thread;
	register rt_ubase_t highest_ready_priority;
	
	highest_ready_priority = __rt_ffs(rt_thread_ready_priority_group) - 1;
	
	to_thread = rt_list_entry(rt_thread_priority_table[highest_ready_priority].next,
	                          struct rt_thread,
														tlist);
	
	rt_current_thread = to_thread;
	rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
#endif
}

/**
 * This function will initialize scheduler
 * 
 * @param 
 * 
 * @return 
 */
void rt_schedule_insert_thread(struct rt_thread *thread)
{
	register rt_base_t temp;
	
	temp = rt_hw_interrupt_disable();
	
	thread->stat = RT_THREAD_READY;
	rt_list_insert_before(&(rt_thread_priority_table[thread->current_priority]), &(thread->tlist));
	rt_thread_ready_priority_group |= thread->number_mask;
	
	rt_hw_interrupt_enable(temp);
}

/**
 * This function will initialize scheduler
 * 
 * @param 
 * 
 * @return 
 */
void rt_schedule_remove_thread(struct rt_thread *thread)
{
	register rt_base_t temp;
	
	temp = rt_hw_interrupt_disable();
	
	rt_list_remove(&(thread->tlist));
	if(rt_list_isempty(&(rt_thread_priority_table[thread->current_priority])))
	{
		rt_thread_ready_priority_group &= ~thread->number_mask;
	}
	
	rt_hw_interrupt_enable(temp);
}

/**
 * This function will initialize scheduler
 * 
 * @param 
 * 
 * @return 
 */
void rt_schedule(void)
{
	struct rt_thread *to_thread;
	struct rt_thread *from_thread;
	
#if 0
	if(rt_current_thread == rt_list_entry(rt_thread_priority_table[0].next, struct rt_thread, tlist))
	{
		from_thread = rt_current_thread;
		to_thread = rt_list_entry(rt_thread_priority_table[1].next, struct rt_thread, tlist);
		rt_current_thread = to_thread;
	}
	else
	{
		from_thread = rt_current_thread;
		to_thread = rt_list_entry(rt_thread_priority_table[0].next, struct rt_thread, tlist);
		rt_current_thread = to_thread;
	}
	
	/* ------------- */

	if(rt_current_thread == &idle)
	{
		if(rt_flag1_thread.remaining_tick == 0)
		{
			from_thread = rt_current_thread;
			to_thread = &rt_flag1_thread;
			rt_current_thread = to_thread;
		}
		else if(rt_flag2_thread.remaining_tick == 0)
		{
			from_thread = rt_current_thread;
			to_thread = &rt_flag2_thread;
			rt_current_thread = to_thread;
		}
		else
		{
			return;
		}
	}
	else
	{
		if(rt_current_thread == &rt_flag1_thread)
		{
			if(rt_flag2_thread.remaining_tick == 0)
			{
				from_thread = rt_current_thread;
				to_thread = &rt_flag2_thread;
				rt_current_thread = to_thread;
			}
			else if(rt_current_thread->remaining_tick != 0)
			{
				from_thread = rt_current_thread;
				to_thread = &idle;
				rt_current_thread = to_thread;
			}
			else
			{
				return;
			}
		}
		else if(rt_current_thread == &rt_flag2_thread)
		{
			if(rt_flag1_thread.remaining_tick == 0)
			{
				from_thread = rt_current_thread;
				to_thread = &rt_flag1_thread;
				rt_current_thread = to_thread;
			}
			else if(rt_current_thread->remaining_tick != 0)
			{
				from_thread = rt_current_thread;
				to_thread = &idle;
				rt_current_thread = to_thread;
			}
			else
			{
				return;
			}
		}
	}
#else

	rt_base_t level;
	register rt_ubase_t highest_ready_priority;
	
	level = rt_hw_interrupt_disable();
	highest_ready_priority = __rt_ffs(rt_thread_ready_priority_group) - 1;
	to_thread = rt_list_entry(rt_thread_priority_table[highest_ready_priority].next, struct rt_thread, tlist);
	
	if(to_thread != rt_current_thread)
	{
		rt_current_priority = (rt_uint8_t)highest_ready_priority;
		from_thread         = rt_current_thread;
		rt_current_thread   = to_thread;
		
		rt_hw_context_switch((rt_uint32_t)&from_thread->sp, 
		                     (rt_uint32_t)&to_thread->sp);
		
		rt_hw_interrupt_enable(level);
	}
	else
	{
		rt_hw_interrupt_enable(level);
	}
	
	rt_hw_interrupt_enable(level);

#endif	
	//rt_hw_context_switch((rt_uint32_t)&from_thread->sp, (rt_uint32_t)&to_thread->sp);
}

