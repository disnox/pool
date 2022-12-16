/*
************************************************************************
* file name: src/main.c
* author: 苏尚宇
* date: Mon Dec 12 14:32:25 CST 2022
* path: /mnt/e/Code/puresoft/thread_pool
* describe: NULL
************************************************************************
*/

#include <stdio.h>
#include "thread_pool.h"

void *my_task(void *arg)
{
	int n = *(int *)arg;

	printf("[%u][%s] ==> job will be done in %d sec...\n", (unsigned)pthread_self(), __FUNCTION__, n);

	sleep(n);

	printf("[%u][%s] ==> job done!\n", (unsigned)pthread_self(), __FUNCTION__);

	return NULL;

	// pthread_exit(NULL);
}

void *count_time(void *arg)
{
	int i = 0;
	while(1) {
		sleep(1);
		printf("sec: %d\n", ++i);
	}
}

int main(int argc, char *argv[])
{
	pthread_t a;
	pthread_create(&a, NULL, count_time, NULL);

	// 1.初始化一个带有2条线程的线程池
	thread_pool *pool = malloc(sizeof(thread_pool));
	init_pool(pool, 1);

	// 2.投入3个任务
	static int temp1 = 1, temp2 = 2, temp3 = 3, temp4 = 4, temp5 = 5;
	printf("throwing 3 tasks...\n");
	add_task(pool, my_task, (void *)&temp1);
	add_task(pool, my_task, (void *)&temp2);
	add_task(pool, my_task, (void *)&temp3);
  
	// 3.显示当前有多少条线程
	printf("current thread number: %d\n", remove_thread(pool, 0));
	// sleep(9);

	// 4.再投入2个任务
	printf("throwing another 2 tasks...\n");
	add_task(pool, my_task, (void *)&temp4);
	add_task(pool, my_task, (void *)&temp5);

	// 5.增加2条线程
	add_thread(pool, 2);
	sleep(5);

	// 6.删除3条线程
	printf("remove 3 threads from the pool...\n");
	printf("current thread number: %d\n", remove_thread(pool, 3));

	// 7.销毁线程池
	destroy_pool(pool);

	return 0;
}
