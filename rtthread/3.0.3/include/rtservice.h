#ifndef __RT_SERVICE_H__
#define __RT_SERVICE_H__

#include <rtthread.h>

/* init list */
rt_inline void rt_list_init(rt_list_t *l)
{
	l->next = l->prev = l;
}

/* insert node after list head */
rt_inline void rt_list_insert_after(rt_list_t *l, rt_list_t *n)
{
	l->next->prev = n;
	n->next = l->next;
	
	l->next = n;
	n->prev = l;
}

/* insert node before list head */
rt_inline void rt_list_insert_before(rt_list_t *l, rt_list_t *n)
{
	l->prev->next = n;
	n->prev = l->prev;
	
	l->prev = n;
	n->next = l;
}

/* remove node from list */
rt_inline void rt_list_remove(rt_list_t *n)
{
	n->next->prev = n->prev;
	n->prev->next = n->next;
	
	n->next = n->prev = n;
}

rt_inline rt_bool_t rt_list_isempty(rt_list_t *l)
{
	if(l->next == l)
	{
		return RT_TRUE;
	}
	return RT_FALSE;
}

#endif
