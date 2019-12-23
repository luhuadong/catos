#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

#include "rtdef.h"

rt_err_t rt_thread_init(struct rt_thread *thread,
                        void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												rt_uint32_t stack_size);

#endif /* __RT_THREAD_H__ */
