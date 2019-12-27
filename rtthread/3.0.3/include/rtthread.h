#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

#include <rtconfig.h>
#include <rtdef.h>
#include <rtservice.h>

#define rt_container_of(ptr, type, member) \
        ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define rt_list_entry(node, type, member) \
        rt_container_of(node, type, member)

extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];
extern struct rt_thread *rt_current_thread;

rt_err_t rt_thread_init(struct rt_thread *thread,
	                      const char *name,
                        void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												rt_uint32_t stack_size);

void rt_tick_increase(void);
void rt_interrupt_enter(void);
void rt_interrupt_leave(void);

/*
 * schedule service
 */
void rt_system_scheduler_init(void);
void rt_system_scheduler_start(void);
void rt_schedule(void);
												
void rt_thread_idle_init(void);
void rt_thread_delay(rt_tick_t tick);

void rt_object_init(struct rt_object *object, enum rt_object_class_type type, const char *name);
												
char *rt_strncpy(char *dst, const char *src, rt_ubase_t n);

#endif /* __RT_THREAD_H__ */
