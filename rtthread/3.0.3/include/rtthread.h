#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

#include "rtdef.h"
#include "rtconfig.h"
#include "rtservice.h"

extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

rt_err_t rt_thread_init(struct rt_thread *thread,
                        void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												rt_uint32_t stack_size);

#endif /* __RT_THREAD_H__ */
