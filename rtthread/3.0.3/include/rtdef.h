#ifndef __RT_DEF_H__
#define __RT_DEF_H__

#include <rtconfig.h>

/* data type */
typedef signed char     rt_int8_t;
typedef signed short    rt_int16_t;
typedef signed long     rt_int32_t;
typedef unsigned char   rt_uint8_t;
typedef unsigned short  rt_uint16_t;
typedef unsigned long   rt_uint32_t;
typedef int             rt_bool_t;

/* 32bit CPU */
typedef long            rt_base_t;
typedef unsigned long   rt_ubase_t;

typedef rt_base_t       rt_err_t;
typedef rt_uint32_t     rt_time_t;
typedef rt_uint32_t     rt_tick_t;
typedef rt_base_t       rt_flag_t;
typedef rt_ubase_t      rt_size_t;
typedef rt_ubase_t      rt_dev_t;
typedef rt_base_t       rt_off_t;

/* bool */
#define RT_TRUE         1
#define RT_FALSE        0

/* error code */
#define RT_EOK          0    /* 没有错误 */
#define RT_ERROR        1    /* 一个常规错误 */
#define RT_ETIMEOUT     2    /* 超时 */
#define RT_EFULL        3    /* 资源已满 */
#define RT_EEMPTY       4    /* 资源为空 */
#define RT_ENOMEM       5    /* 没有内存 */
#define RT_ENOSYS       6    /* No system */
#define RT_EBUSY        7    /* Busy */
#define RT_EIO          8    /* IO error */
#define RT_EINTR        9    /* 中断系统调用 */
#define RT_EINVAL       10   /* 无效形参 */

/* thread stat */
#define RT_THREAD_INIT                  0x00               /* 初始态 */
#define RT_THREAD_READY                 0x01               /* 就绪态 */
#define RT_THREAD_SUSPEND               0x02               /* 挂起态 */
#define RT_THREAD_RUNNING               0x03               /* 运行态 */
#define RT_THREAD_BLOCK                 RT_THREAD_SUSPEND  /* 阻塞态 */
#define RT_THREAD_CLOSE                 0x04               /* 关闭态 */
#define RT_THREAD_STAT_MASK             0x0f

#define RT_THREAD_STAT_SIGNAL           0x10
#define RT_THREAD_STAT_SIGNAL_READY     (RT_THREAD_STAT_SIGNAL | RT_THREAD_READY)
#define RT_THREAD_STAT_SIGNAL_SUSPEND   0x20
#define RT_THREAD_STAT_SIGNAL_MASK      0xf0

/* timer stat */
#define RT_TIMER_FLAG_DEACTIVATED       0x0
#define RT_TIMER_FLAG_ACTIVATED         0x1

#define RT_TIMER_FLAG_ONE_SHOT          0x0
#define RT_TIMER_FLAG_PERIODIC          0x2

#define RT_TIMER_FLAG_HARD_TIMER        0x0
#define RT_TIMER_FLAG_SOFT_TIMER        0x4

#define RT_TIMER_CTRL_GET_TIME          0x0
#define RT_TIMER_CTRL_SET_TIME          0x1
#define RT_TIMER_CTRL_SET_ONESHOT       0x2
#define RT_TIMER_CTRL_SET_PERIODIC      0x3

#define RT_TICK_MAX                     0xffffffff

/**
 * clock & timer macros
 */
#define RT_TIMER_SKIP_LIST_LEVEL    1
#define RT_TIMER_SKIP_LIST_MASK     0x3


#ifdef __CC_ARM
#define rt_inline       static __inline
#define ALIGN(n)        __attribute__((aligned(n)))

#elif defined (__IAR_SYSTEMS_ICC__)
#define rt_inline       static inline
#define ALIGN(n)        PRAGMA(data_alignment=n)

#elif defined (__GNUC__)
#define rt_inline       static __inline
#define ALIGN(n)        __attribute__((aligned(n)))

#else
#error not supported tool chain
#endif

#define RT_ALIGN(size, align)       (((size) + (align)-1) & ~((align)-1))
#define RT_ALIGN_DOWN(size, align)  ((size) & ~((align)-1))

#define RT_NULL         (0)


enum rt_object_class_type
{
	RT_Object_Class_Thread = 0,
	RT_Object_Class_Semaphore,
	RT_Object_Class_Mutex,
	RT_Object_Class_Event,
	RT_Object_Class_MailBox,
	RT_Object_Class_MessageQueue,
	RT_Object_Class_MemHeap,
	RT_Object_Class_MemPool,
	RT_Object_Class_Device,
	RT_Object_Class_Timer,
	RT_Object_Class_Module,
	RT_Object_Class_Unknown,
	RT_Object_Class_Static = 0x80
};

struct rt_list_node
{
	struct rt_list_node *next;  /* point to the next node */
	struct rt_list_node *prev;  /* point to the prev node */
};
typedef struct rt_list_node rt_list_t;

struct rt_object
{
	char name[RT_NAME_MAX];
	rt_uint8_t type;
	rt_uint8_t flag;
	
	rt_list_t list;
};
typedef struct rt_object *rt_object_t;

struct rt_object_information
{
	enum rt_object_class_type type;
	rt_list_t                 object_list;
	rt_size_t                 object_size;
};

struct rt_timer
{
	struct rt_object parent;
	
	rt_list_t row[RT_TIMER_SKIP_LIST_LEVEL];
	
	void (*timeout_func)(void *parameter);
	void *parameter;
	
	rt_tick_t init_tick;
	rt_tick_t timeout_tick;
};
typedef struct rt_timer *rt_timer_t;

struct rt_thread
{
	/* rt object */
	char        name[RT_NAME_MAX];  /* name */
	rt_uint8_t  type;               /* type */
	rt_uint8_t  flag;               /* status */
	rt_list_t   list;               /* object list node */
	
	rt_list_t   tlist;              /* thread list node */
	void        *sp;                /* thread stack pointer */
	void        *entry;             /* thread entry address */
	void        *parameter;         /* thread formal parameter */
	void        *stack_addr;        /* the stack address */
	rt_uint32_t stack_size;         /* stack size (byte) */
	
	rt_ubase_t  remaining_tick;     /* block delay */
	
	rt_uint8_t  current_priority;   /* current priority */
	rt_uint8_t  init_priority;      /* initial priority */
	rt_uint32_t number_mask;        /* mask for current priority */
	                                /* equal the index on rt_thread_ready_priority_group */
	
	rt_err_t    error;              /* error code */
	rt_uint8_t  stat;               /* thread stat */
	
	struct rt_timer thread_timer;   /* thread timer */
};
typedef struct rt_thread *rt_thread_t;


#endif /* __RT_DEF_H__ */
