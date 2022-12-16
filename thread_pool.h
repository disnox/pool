#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <pthread.h>

#define MAX_WAITING_TASKS	1000
#define MAX_ACTIVE_THREADS	20

struct task 	// 任务节点
{
	void *(*task)(void *arg);
	void *arg;

	struct task *next;
};

typedef struct thread_pool	// 线程池
{
	pthread_mutex_t lock;	// 互斥锁，保护任务队列
	pthread_cond_t  cond;	// 条件变量，同步所有线程
	struct task *task_list;	// 任务链队列指针

	pthread_t *tids;		// 线程ID存放位置

	unsigned waiting_tasks;	// 任务链队列中等待的任务个数
	unsigned active_threads;// 当前活跃线程个数

	bool shutdown;			// 线程池销毁标记
}thread_pool;

// 线程池初始化
bool init_pool(thread_pool *pool, unsigned int threads_number);

// 投放任务
bool add_task(thread_pool *pool, void *(*task)(void *arg), void *arg);

// 添加线程
int add_thread(thread_pool *pool, unsigned int additional_threads_number);

// 删除线程
int remove_thread(thread_pool *pool, unsigned int removing_threads);

// 销毁线程池
bool destroy_pool(thread_pool *pool);

// 线程例程
void *routine(void *arg);

#endif
