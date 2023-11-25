#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


void *func(void *arg){

	int count = 0;

	while(1){
		printf("-------------------------------------------- count = %d\n", count);
		sleep(1);
		count ++;
	}
	return NULL;
}



int main(int argc, const char *argv[])
{
	pthread_t tid;

	int ret;

	ret = pthread_create(&tid, NULL, func, NULL);
	if (0 != ret){
		fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
	}
	printf("main thread: pid = %d   tid = %ld\n", getpid(), pthread_self());
	
	ret = pthread_detach(tid);
	if (ret != 0){
		fprintf(stderr, "pthread_detach error: %s\n", strerror(ret));
	}
	ret = pthread_join(tid, NULL);
	if (0 != ret){
		fprintf(stderr, "pthread_join error: %s\n", strerror(ret));
	}
	pthread_exit((void *)0);
}
