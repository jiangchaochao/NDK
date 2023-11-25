#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int count = 0;

void *func(void *arg){
	while(1){
		printf("-------------------------------------------- count = %d\n", count);
		sleep(1);
		count ++;
	}
	return NULL;
}


void *func1(void *arg){
	while(1){
		pthread_testcancel();
	}
}

int main(int argc, const char *argv[])
{
	pthread_t tid;

	int ret;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&tid, &attr, func1, NULL);
	if (0 != ret){
		fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
	}
	printf("main thread: pid = %d   tid = %ld\n", getpid(), pthread_self());
	pthread_attr_destroy(&attr);
	sleep(5);
	ret = pthread_cancel(tid);
	if (0 != ret){
		fprintf(stderr, "pthread_cancel error: %s\n", strerror(ret));
	}
	ret = pthread_join(tid, NULL);
	if (0 != ret){
		fprintf(stderr, "pthread_join error: %s\n", strerror(ret));
	}


	printf("main thread exit\n");
	
	pthread_exit((void *)0);
}
