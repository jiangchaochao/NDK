#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *func(void *arg){

	printf("child pthread: pid = %d   tid = %ld\n", getpid(), pthread_self());
	printf("-----------------------------\n");
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
	sleep(3);
	return 0;
}
