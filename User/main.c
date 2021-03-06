/*
 */
 
#include <rtthread.h>
#include <rthw.h>
#include "ARMCM3.h"

/* define thread control block */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;
struct rt_thread rt_flag3_thread;

ALIGN(RT_ALIGN_SIZE)

/* stack of thread */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];
rt_uint8_t rt_flag3_thread_stack[512];

/* function declaration */
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);
void flag3_thread_entry(void *p_arg);

rt_uint8_t flag1, flag2, flag3;

extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];


void delay(rt_uint32_t count)
{
	for (; count!=0; count--);
}

void SysTick_Handler(void)
{
	rt_interrupt_enter();
	rt_tick_increase();
	rt_interrupt_leave();
}

int main(void)
{
	/* 硬件初始化 */
	/* 将硬件相关的初始化放在这里，如果是软件仿真则没有初始化代码 */
	
	
	/* 关中断 */
	rt_hw_interrupt_disable();
	
	/* SysTick中断频率设置，调用固件库函数SysTick_Config() */
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
	
	rt_system_timer_init();
	
	/* initialized scheduler */
	rt_system_scheduler_init();
	
	/* initialized idle thread */
	rt_thread_idle_init();
	
	/* initialized thread */
	rt_thread_init(&rt_flag1_thread,
	               "t_flag1",
	               flag1_thread_entry,
								 RT_NULL,
	               &rt_flag1_thread_stack[0],
	               sizeof(rt_flag1_thread_stack),
								 2,
								 4);
	
	//rt_list_insert_before(&(rt_thread_priority_table[0]), &(rt_flag1_thread.tlist));
	rt_thread_startup(&rt_flag1_thread);
	
	rt_thread_init(&rt_flag2_thread,
	               "t_flag2",
	               flag2_thread_entry,
								 RT_NULL,
	               &rt_flag2_thread_stack[0],
	               sizeof(rt_flag2_thread_stack),
								 3,
								 2);
	
	//rt_list_insert_before(&(rt_thread_priority_table[1]), &(rt_flag2_thread.tlist));
	rt_thread_startup(&rt_flag2_thread);
								 
	rt_thread_init(&rt_flag3_thread,
	               "t_flag3",
	               flag3_thread_entry,
								 RT_NULL,
	               &rt_flag3_thread_stack[0],
	               sizeof(rt_flag3_thread_stack),
								 3,
								 3);
	rt_thread_startup(&rt_flag3_thread);
	
	rt_system_scheduler_start();
	
}

void flag1_thread_entry(void *p_arg)
{
	for (;;)
	{
#if 0
		flag1 = 1;
		delay(100);
		flag1 = 0;
		delay(100);
		
		rt_schedule();
#else
		flag1 = 1;
		rt_thread_delay(3);
		flag1 = 0;
		rt_thread_delay(3);
#endif
	}
}

void flag2_thread_entry(void *p_arg)
{
	for (;;)
	{
#if 0
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
		
		rt_schedule();
#else
		flag2 = 1;
		//rt_thread_delay(2);
		delay(100);
		flag2 = 0;
		//rt_thread_delay(2);
		delay(100);
#endif
	}
}

void flag3_thread_entry(void *p_arg)
{
	for (;;)
	{
		flag3 = 1;
		//rt_thread_delay(3);
		delay(100);
		flag3 = 0;
		//rt_thread_delay(3);
		delay(100);
	}
}
