#include "rtdef.h"

/* init list */
rt_inline void rt_list_init(rt_list_t *l)
{
	l->next = l->prev = l;
}

/* insert node */
rt_inline void rt_list_insert_after(rt_list_t *l, rt_list_t *n)
{
}
