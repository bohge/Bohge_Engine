// pThreadTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <windows.h>


using namespace std;

string SharedData;

void * thread_function_r(void *arg); //线程函数
/*
信号量的数据类型为结构sem_t，它本质上是一个长整型的数。函数sem_init（）用来初始化一个信号量。它的原型为：　　
extern int sem_init __P ((sem_t *__sem, int __pshared, unsigned int __value));　　
__sem为指向信号量结构的一个指针；__pshared不为０时此信号量在进程间共享，否则只能为当前进程的所有线程共享；__value给出了信号量的初始值。　　
*/
sem_t sem_r, sem_w; //semphore，信号量，本程序用于保证进程和线程间的同步
pthread_mutex_t mutex; //互斥锁，保证线程访问的互斥，步骤一，定义数据类型
int main(int argc, char *argv[])
{
	int res;
	pthread_t a_thread_r, b_thread_r; //线程ID变量，与进程类似，进程为pid_t

	/*信号量用sem_init函数创建的，下面是它的说明：
	　　#include<semaphore.h>
	  int sem_init (sem_t *sem, int pshared, unsigned int value);
	  这个函数的作用是对由sem指定的信号量进行初始化，设置好它的共享选项，并指定一个整数类型的初始值。pshared参数控制着信号量的类型。如果 pshared的值是０，就表示它是当前里程的局部信号量；否则，其它进程就能够共享这个信号量。
	  */
	res = sem_init(&sem_r, 0, 0);
	if ( res != 0) 
	{
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	res = sem_init(&sem_w, 0, 2);
	if ( res != 0) 
	{
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	if ( pthread_mutex_init(&mutex, NULL) != 0 ) //互斥锁，保证线程访问的互斥，步骤二，初始化，默认属性初始化互斥量——NULL参数的功能
	{
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}


	for ( int i = 0 ; i < 10 ; i ++)
	{
		pthread_create(&a_thread_r, NULL, thread_function_r, new int(i) );
	}
	while(1) //【功能】主进程执行写入操作
	{
		sem_wait(&sem_r); //函数sem_wait( sem_t *sem )被用来阻塞当前线程直到信号量sem的值大于0，解除阻塞后将sem的值减一，表明公共资源经使用后减少。函数sem_trywait ( sem_t *sem )是函数sem_wait（）的非阻塞版本，它直接将信号量sem的值减一。函数sem_destroy(sem_t *sem)用来释放信号量sem。
		pthread_mutex_lock(&mutex);
		if( 0 != SharedData.size() )
		{
			cout<<SharedData.c_str();
			SharedData.clear();
		}
		else
		{
			cout<<"Empty data\n";
		}
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_w); //函数sem_post( sem_t *sem )用来增加信号量的值。当有线程阻塞在这个信号量上时，调用这个函数会使其中的一个线程不再阻塞，选择机制同样是由线程的调度策略决定的。
		Sleep(1);
	}
	return 0;
}
void * thread_function_r(void *arg) //【功能】线程负责读取
{
	int i = *((int*)arg);
	string num;
	num.resize( 1 );
	num[0] = '0' + i;
	while(1)
	{
		sem_wait(&sem_r);
		pthread_mutex_lock(&mutex); //获取互斥量（互斥锁），另外有pthread_mutex_trylock尝试对互斥量加锁，如果失败返回EBUSY
		if ( 0 == SharedData.size() )
		{
			SharedData = "in ";
			SharedData += num;
			SharedData += " function wirted\n";
		}
		else
		{
			cout<<"Avoid over write ather's data\n";
		}
		pthread_mutex_unlock(&mutex); //释放互斥锁
		sem_post(&sem_w);
		Sleep(1);
	}
}