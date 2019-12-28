#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

#include <rtconfig.h>
#include <rtdef.h>
#include <rtservice.h>

#define rt_container_of(ptr, type, member) \
        ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define rt_list_entry(node, type, member) \
        rt_container_of(node, type, member)

extern rt_uint32_t rt_thread_ready_priority_group;
extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];
extern struct rt_thread *rt_current_thread;

rt_err_t rt_thread_init(struct rt_thread *thread,
	                      const char       *name,
                        void (*entry)(void *parameter),
												void             *parameter,
												void             *stack_start,
												rt_uint32_t      stack_size,
												rt_uint8_t       priority,
												rt_uint32_t      tick);

rt_err_t rt_thread_startup(rt_thread_t thread);
rt_err_t rt_thread_resume(rt_thread_t thread);
rt_err_t rt_thread_suspend(rt_thread_t thread);
rt_err_t rt_thread_yield(void);
void rt_thread_timeout(void *parameter);
rt_thread_t rt_thread_self(void);
												
void rt_thread_idle_init(void);
rt_err_t rt_thread_delay(rt_tick_t tick);
rt_err_t rt_thread_sleep(rt_tick_t tick);
												
void rt_tick_increase(void);
void rt_interrupt_enter(void);
void rt_interrupt_leave(void);

/*
 * schedule service
 */
void rt_system_scheduler_init(void);
void rt_system_scheduler_start(void);
void rt_schedule_insert_thread(struct rt_thread *thread);
void rt_schedule_remove_thread(struct rt_thread *thread);
void rt_schedule(void);

void rt_object_init(struct rt_object *object, enum rt_object_class_type type, const char *name);

rt_tick_t rt_tick_get(void);

/* timer */
void rt_system_timer_init(void);
void rt_timer_init(rt_timer_t timer,
	                 const char *name,
									 void (*timeout)(void *parameter),
									 void       *parameter,
									 rt_tick_t  time,
									 rt_uint8_t flag);

rt_err_t rt_timer_start(rt_timer_t timer);
rt_err_t rt_timer_stop(rt_timer_t timer);
rt_err_t rt_timer_control(rt_timer_t timer, int cmd, void *arg);
void rt_timer_check(void);

char *rt_strncpy(char *dst, const char *src, rt_ubase_t n);
int __rt_ffs(int value);

#endif /* __RT_THREAD_H__ */
