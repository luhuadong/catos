#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

#include "rtdef.h"
#include "rtconfig.h"
#include "rtservice.h"
#include "rthw.h"


rt_err_t rt_thread_init(struct rt_thread *thread,
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

#endif /* __RT_THREAD_H__ */
