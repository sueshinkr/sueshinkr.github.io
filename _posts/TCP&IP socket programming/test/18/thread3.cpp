// thread3.cpp

#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void*	thread_summation(void *arg);
int		sum = 0;

int main(int argc, char *argv[])
{
	pthread_t	id_t1, id_t2;
	int			range1[] = {1, 5};
	int			range2[] = {6, 10};

	pthread_create(&id_t1, NULL, thread_summation, (void*)range1);
	pthread_create(&id_t2, NULL, thread_summation, (void*)range2);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);
	cout << "result : " << sum << endl;

	return 0;
}

void*	thread_summation(void *arg)
{
	int	start = ((int*)arg)[0];
	int	end = ((int*)arg)[1];

	while (start <= end)
	{
		sum += start;
		start++;
	}

	return NULL;
}