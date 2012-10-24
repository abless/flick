/*
 * include/kernel/list.h
 * Copyright (C) 2003 Alexander Blessing
 * Copyright (C) 2003 Pekka Enberg
 *
 * Not invented here. Please refer to <linux/list.h>
 */

#ifndef _INCLUDE_KERNEL_LIST_H_
#define _INCLUDE_KERNEL_LIST_H_

#include <stdio.h>

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

static inline struct list_head *list_remove(struct list_head *list_head)
{
	list_head->prev->next = list_head->next;
	list_head->next->prev = list_head->prev;
	list_head->prev = NULL; list_head->next = NULL;
	return list_head;
}

static inline int list_is_empty(struct list_head *list)
{
	return (list->next == list) && (list->prev == list);
}

#define list_queue_insert(queue, item, member) \
	do { \
		(item)->member.prev = queue; \
		(item)->member.next = (queue)->next; \
		(queue)->next->prev = &((item)->member); \
		(queue)->next = &((item)->member); \
	} while (0);

#define list_entry(list_head, type, member) \
	((type *)((char *)(list_head) - offsetof(type, member)))

#define list_queue_remove(queue, type, member) \
	list_entry(list_remove((queue)->prev), type, member)

#endif
