/*
 * include/kernel/process.h
 * Copyright (C) 2003 Alexander Blessing
 */

#ifndef _INCLUDE_KERNEL_PROCESS_H_
#define _INCLUDE_KERNEL_PROCESS_H_

#include <kernel/list.h>
#include <types.h>

#define SCHED_ENABLE 0
#define SCHED_DISABLE 1

#define MAX_PID 1024
#define MAX_PROC 32
#define MAX_PRIOR 3

/* process level */
#define PROC_KERNEL 0
#define PROC_USER 1

/* process priority */
#define SCHED_LOW 0
#define SCHED_NORMAL 1 
#define SCHED_HIGH 2

/* process state */
#define RUNNING 0
#define READY 1
#define BLOCKED 2

typedef struct process process_t;

/* context used for placing registers on the stack */
typedef struct {
	int gs;
	int fs;
	int es;
	int ds;
	
	int edi;
	int esi;
	int ebp;
	int esp;
	int ebx;
	int edx;
	int ecx;
	int eax;

	int eip;
	int cs;
	int eflags;
} context;

/* user context */
typedef struct {
	context ctx;
	int user_esp, user_ss;
} usr_context;

struct process {
	pid_t pid;
	int level;	/* kernel/user */
	int state;	/* running/waiting */
	int prior;	/* priority */
	int del;	
	int run;
	void *base;	
	void *stack;	/* pointer to SP */
	
	struct list_head list_head;	/* for queue */
};

typedef struct {
	process_t *head, *tail;
} pqueue_t;

/* prototypes */
void set_sched(int);
void *schedule(void);
void init_proc(void);
pid_t create_process(int, int, int);
int pexit(int);

inline int queue_empty(pqueue_t *);

#endif
