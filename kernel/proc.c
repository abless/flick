/*
 * kernel/proc.c
 * Copyright (C) 2003 Alexander Blessing
 *
 * handles process management
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <kernel/process.h>
#include <kernel/list.h>

extern int CS_SELECTOR, DS_SELECTOR;

int sched_enable = 0;

struct list_head process_queue[MAX_PRIOR];
process_t *pid_queue[MAX_PID];
process_t *current;

void init_proc(void)
{
	int i;

	for (i = 0; i < MAX_PID; i++)
		pid_queue[i] = NULL;
	for (i = 0; i < MAX_PRIOR; i++)  
		/* initialize process queues */
		LIST_HEAD_INIT(&process_queue[i]);
	current = NULL;
}

void fill_context(process_t *proc, int eip, int state)
{
	context *ctx;
	
	switch(state) {
		case PROC_KERNEL:
			/* allocate stack */
			proc->base = malloc(8192);
			proc->stack = (void*) ((int) proc->base + 8192 - sizeof(context));
			/* set up context on the stack */
			ctx = (context *) proc->stack;
			memset(ctx, 0, sizeof(context));
						
			ctx->ds = ctx->es = ctx->fs = ctx->gs = (int) &DS_SELECTOR;
			ctx->cs = (int) &CS_SELECTOR;
			ctx->eip = eip;
			ctx->eflags = 0x202;	/* IF flag set */
			break;
		default:
			panic("unsupported process type");
	}
}

pid_t create_process(int func, int prior, int level)
{
	process_t *proc;
	pid_t pid;

	proc = malloc(sizeof(process_t));	/* allocate process structure */
	memset(proc, 0, sizeof(proc));
	
	/* assign PID to process */
	for (pid = 0; pid < MAX_PID; pid++)
		if (pid_queue[pid] == NULL)
			break;
	if (pid == (MAX_PID - 1))	/* too many PIDs */
		panic("no more free PID");
	pid_queue[pid] = proc;
	proc->pid = pid;
	
	/* fill process context */
	fill_context(proc, func, level);
	
	/* set attributes */
	proc->level = level;
	proc->state = READY;
	proc->prior = prior;
	proc->del = 0;
	proc->run = 0;	/* process hasn't run yet */
	
	/* insert into queue */
	list_queue_insert(&process_queue[prior], proc, list_head);
	return pid;
}

/* kill a process */
int process_kill(pid_t pid)
{
	process_t *p;
	
	if (pid < 0 || pid > MAX_PID)
		return -1;
	p = pid_queue[pid];
	if (!p)
		return -1;	/* couldn't find process */
	
	p->del = 1;	/* set deletion flag */
	return 0;
}

int pexit(int status)
{
	process_kill(current->pid);
	
	/* wait for scheduler to delete this process
	 * TODO: reschedule */
	while(1);	
}

void process_delete(process_t *p)
{
	pid_queue[p->pid] = NULL;
	free(p->base);
	free(p);
}

/* our scheduler */
void *schedule(void)
{
	process_t *p;
	int i;
	
	if (list_is_empty(&process_queue[0]) && list_is_empty(&process_queue[1])		&& list_is_empty(&process_queue[2]))
			panic("no runnable process");

	current->state = READY;	/* not longer running */
	do {
		for (i = MAX_PRIOR - 1; i >= 0; i--) 
			if (!list_is_empty(&process_queue[i]))
				break;
		if (list_is_empty(&process_queue[i]))
			panic("no runnable process");
		
		/* pick next process to run */
		p = list_queue_remove(&process_queue[i], process_t, list_head);
		if (p->del == 1) {	/* process marked for deletion */
			process_delete(p);
			continue;	/* pick new process */
		}
		/* re-insert process at tail of queue */
		list_queue_insert(&process_queue[i], p, list_head);
	} while(p->state != READY);
	current = p;
	
	p->state = RUNNING;
	p->run = 1;	

	return p->stack;	/* return stack pointer */
}

void set_sched(int flag)
{
	switch (flag) {
		case SCHED_ENABLE:
			sched_enable = 1;
			break;
		case SCHED_DISABLE:
			sched_enable = 0;
			break;
	}
}
