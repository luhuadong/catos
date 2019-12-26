#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

#include "rtdef.h"
#include "rtconfig.h"
#include "rtservice.h"
#include "rthw.h"


rt_err_t rt_thread_init(struct rt_thread *thread,
	                      const char *name,
                        void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												rt_uint32_t stack_size);
												
/*
 * schedule service
 */
void rt_system_scheduler_init(void);
void rt_system_scheduler_start(void);
void rt_schedule(void);

void rt_object_init(struct rt_object *object, enum rt_object_class_type type, const char *name);
												
char *rt_strncpy(char *dst, const char *src, rt_ubase_t n);

#endif /* __RT_THREAD_H__ */
