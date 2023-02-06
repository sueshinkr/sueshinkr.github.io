// thread1.cpp

#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void*	thread_main(void *arg);

int main(int argc, char *argv[])
{
	pthread_t	t_id;
	int			thread_param = 5;

	if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0)
	{
		cout << "pthread_create() error\n";
		return -1;
	}

	sleep(10);
	cout << "end of main\n";
	return 0;
}

void*	thread_main(void *arg)
{
	int	cnt = *((int*)arg);
	for (int i = 0; i < cnt; i++)
	{
		sleep(1);
		cout << "running thread\n";
	}
	return NULL;
}