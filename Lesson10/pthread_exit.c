#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void tesxxx(){


	// ....
	printf("need exit..................\n");

	char *p = malloc(128);
	memcpy(p, "hello world", sizeof("hello world"));
	pthread_exit((void *)p);
}

void *func(void *arg){

	printf("child pthread: pid = %d   tid = %ld\n", getpid(), pthread_self());
	printf("-----------------------------\n");
	tesxxx();

	printf("---------------------------------------------------------------------------\n");
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
	
	char *r;

	ret = pthread_join(tid, (void **)&r);
	if (0 != ret){
		fprintf(stderr, "pthread_join error: %s\n", strerror(ret));
	}else{

		printf("r = %s\n", r);
	}

	if (NULL != r){
		free(r);
	}


	return 0;
}
