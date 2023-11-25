# NDK之线程

#### 一. 什么是线程？

这个基本不用多说了吧，学过编程的都知道，但还是简单介绍以下。线程是CPU调度的最小单位，被包含在进程之中，拥有自己的栈空间，和进程共享堆等其他资源，所以在多线程环境中存在线程同步，互斥等问题。



###### 小知识点

其实在Linux中，新建的线程并不是在原先的进程中，而是系统通过一个系统调用clone( )。该系统调用copy了一个和原先进程完全一样的进程，并在这个进程中执行线程函数。不过这个copy过程和fork不一样。 copy后的进程和原先的进程共享了所有的变量，运行环境。这样，原先进程中的变量变动在copy后的进程中便能体现出来。



##### 提示

本次课程使用ubuntu 环境 、vim编辑器

编译命令：

```shell
gcc xxx.c -lpthread

# 执行
./a.out
```



#### 二. 线程的创建函数pthread_create

用于创建一个线程，对应进程中就是fork函数。

```c
#include <pthread.h>
int pthread_create(pthread_t *tidp,const pthread_attr_t *attr, void *(*start_rtn)(void*),void *arg);


// 若线程创建成功，则返回0。若线程创建失败，则返回出错编号，并且*thread中的内容是未定义的。
// 返回成功时，由tidp指向的内存单元被设置为新创建线程的线程ID.attr参数用于指定各种不同的线程属性。新创建的线程从start_rtn函数的地址开始运行，该函数只有一个万能指针参数arg，如果需要向start_rtn函数传递的参数不止一个，那么需要把这些参数放到一个结构中，然后把这个结构的地址作为arg的参数传入。
// 失败返回errno
// 参数
// 第一个参数为指向线程标识符的指针。
// 第二个参数用来设置线程属性。
// 第三个参数是线程运行函数的起始地址。
// 最后一个参数是运行函数的参数。
```



例子：

```c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


void *func(void *arg){

    printf("func------------------\n");
}


int main(int argc, const char *argv[])
{

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, func, NULL);
    if (ret != 0){
        fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
        return 1;
    }
                                                                                          
    sleep(3);
    return 0;
}   

```



#### 三. 线程ID获取函数pthread_self

进程有进程ID，用getpid()。线程同样也有线程ID，对应的函数就是pthread_self函数,用来获取当前线程ID

```c
#include <pthread.h>
// 返回值：成功线程ID；失败:其他
pthread_t pthread_self();

```

例子：

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

void *func(void *arg){
    printf("child pthread: pid = %d tid = %ld\n", getpid(), pthread_self());
    printf("func------------------\n");
}

int main(int argc, const char *argv[])
{

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, func, NULL);
    if (ret != 0){
        fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
        return 1;
    }

    printf("main thread: pid = %d   tid = %ld\n", getpid(), pthread_self());

    sleep(3);
    return 0;                                                                                             
}   

```



#### 四. 线程退出函数pthread_exit函数

线程通过调用pthread_exit函数终止执行，就如同进程在结束时调用exit函数一样。这个函数的作用是，终止调用它的线程并返回一个指向某个对象的指针。

```c
 void pthread_exit(void* retval);
```

例子：

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>



void testxxx()
{

    // ...
    printf("need exit.............  [%s][%d] \n", __func__, __LINE__);
    pthread_exit((void *)100);
}


int test_return(){

    // ....
    printf("need exit ...........................[%s][%d]\n", __func__, __LINE__);
    return 100;

}

void *func(void *arg){
    printf("child pthread: pid = %d tid = %ld\n", getpid(), pthread_self());
    printf("func------------------\n");
    test_return();
    testxxx();

    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
}


int main(int argc, const char *argv[])
{

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, func, NULL);
    if (ret != 0){
        fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
        return 1;
    }

    printf("main thread: pid = %d   tid = %ld\n", getpid(), pthread_self());

    // sleep(3);

    pthread_exit((void *)0);                                                                    

    return 0;
}                                                                 
```





#### 五. pthread_join函数

等待线程的结束，线程间同步的操作。以阻塞的方式等待thread指定的线程结束。当函数返回时，被等待线程的资源被收回。如果线程已经结束，那么该函数会立即返回。并且thread指定的线程必须是joinable的。

```c

 int pthread_join(pthread_t thread, void **retval);

// 参数 ：thread: 线程标识符，即线程ID，标识唯一线程。
// retval: 用户定义的指针，用来存储被等待线程的返回值。
// 返回值 ： 0代表成功。 失败，返回的则是errno。
```

```C
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>



void testxxx()
{
    // ...
    printf("need exit.............  [%s][%d] \n", __func__, __LINE__);
    pthread_exit((void *)100);
}


int test_return(){

    // ....
    printf("need exit ...........................[%s][%d]\n", __func__, __LINE__);
    return 100;

}

void *func(void *arg){
    printf("child pthread: pid = %d tid = %ld\n", getpid(), pthread_self());
    printf("func------------------\n");
    test_return();
    testxxx();

    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
}


int main(int argc, const char *argv[])
{

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, func, NULL);
    if (ret != 0){
        fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
        return 1;
    }

    printf("main thread: pid = %d   tid = %ld\n", getpid(), pthread_self());

    // sleep(3);

    int r;
    ret = pthread_join(tid, &r);
    if (0 != ret){
        fprintf(stderr, "pthread_join error: %s\n", strerror(ret));
    }else{

        printf("r = %d\n", (int)r);

    }

    printf("main end\n");                                                                          

    return 0;
}
```

#### 六. pthread_cancel函数

终止一个线程

```c
int pthread_cancel(pthread_t thread);
// 参数：线程标识符，即线程ID，标识唯一线程。
// 返回值：0代表成功。 失败，返回的则是errno。
```

例子：

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

void *func(void *arg){
    while(1){
        printf("child pthread: pid = %d tid = %ld\n", getpid(), pthread_self());

        sleep(1);
    }
    return NULL;
}


void *fun_1(void *arg){
    while(1){
        pthread_testcancel();
    }
    return (void *)111;
}


int main(int argc, const char *argv[])                                                            
{

    pthread_t tid;
// int ret = pthread_create(&tid, NULL, func, NULL);
    int ret = pthread_create(&tid, NULL, fun_1, NULL);
    if (ret != 0){
        fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
        return 1;
    }   
    
    printf("main thread: pid = %d   tid = %ld\n", getpid(), pthread_self());
    sleep(3);
    ret = pthread_cancel(tid);
    if (0 != ret){
        fprintf(stderr, "pthread_cancel error: %s\n", strerror(ret));
    }   
    
    sleep(1);
    int r;
    ret = pthread_join(tid, &r);
    if (0 != ret){
        fprintf(stderr, "pthread_join error: %s\n", strerror(ret));
    }else{
        printf("r = %d\n", r);
    }   
    
    printf("main end\n");
    
    return 0;
}   
```

###### 小提示

必须要有系统调用，线程必须到达一个取消点才可以杀死线程。可以主动添加一个取消点：pthread_testcancel();

#### 七. pthread_detach函数

线程分离函数，线程结束后，直接自动释放，不用主动回收。

```c
int pthread_detach(pthread_t thread);
// 参数：线程标识符，即线程ID，标识唯一线程。
// 返回值：0代表成功。 失败，返回的则是errno。
```



例子：

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

void *func(void *arg){

    int count = 0;
    while(1){
        if (count == 10){
            printf("-----------------------------------\n");
            return NULL;
        }
        printf("child pthread: pid = %d tid = %ld\n", getpid(), pthread_self());

        sleep(1);
        count ++;
    }
    return NULL;
}

int main(int argc, const char *argv[])
{

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, func, NULL);
    if (ret != 0){
        fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
        return 1;
    }

    ret = pthread_detach(tid);
    if (ret != 0){
        fprintf(stderr, "pthread_detach error: %s\n", strerror(ret));
    }

    ret = pthread_join(tid, NULL);
    if (ret != 0){
        fprintf(stderr, "pthread_join error: %s\n", strerror(ret));
    }
    printf("main thread: pid = %d   tid = %ld\n", getpid(), pthread_self());

    pthread_exit((void *)0);
    return 0;
}                                                                                                     

```







#### 八. 线程属性pthread_attr_t

做简单了解：通过线程属性设置线程分离

```c
typedef struct{
	int                        etachstate;            // 线程的分离状态
	int                        schedpolicy;           // 线程的调度策略
	struct sched_param         schedparam;            // 线程的调度参数
	int                        inheritsched;          // 线程的继承性
	int                        scope;                 // 线程的作用域
	size_t                     guardsize;             // 线程栈末尾的警戒缓冲区大小
	int                        stackaddr_set;         // 线程的栈设置
	void *                     stackaddr;             // 线程栈的位置
	size_t                     stacksize;             // 线程栈大小
}pthread_attr_t;



// 线程属性初始化: 成功：0  失败：errno
int pthread_attr_init(pthread_attr_t *attr);
// 销毁线程属性所占用的资源: 成功0  失败：errno
int pthread_attr_destroy(pthread_attr_t *attr);

// 设置线程属性：是否分离
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
// 获取线程属性：是否分离
int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate);


// detachstate:
// PTHREAD_CREATE_DETACHED: 分离线程
// PTHREAD_CREATE_JOINABLE： 非分离线程




```

例子：

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

void *func(void *arg){

    int count = 0;
    while(1){
        if (count == 10){
            printf("-----------------------------------\n");
            return NULL;
        }
        printf("child pthread: pid = %d tid = %ld\n", getpid(), pthread_self());

        sleep(1);
        count ++;
    }
    return NULL;
}

int main(int argc, const char *argv[])
{

    pthread_t tid;

    int ret;
    pthread_attr_t attr;
    ret = pthread_attr_init(&attr);
    if (ret != 0){
        fprintf(stderr, "pthread_attr_init error: %s\n", strerror(ret));
    }

    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (ret != 0){
        fprintf(stderr, "pthread_attr_setdetachstate error: %s\n", strerror(ret));
    }




    ret = pthread_create(&tid, &attr, func, NULL);
    if (ret != 0){
        fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
        return 1;
    }

    ret = pthread_detach(tid);                                                                 
    if (ret != 0){
        fprintf(stderr, "pthread_detach error: %s\n", strerror(ret));
    }

    ret = pthread_join(tid, NULL);
    if (ret != 0){
        fprintf(stderr, "pthread_join error: %s\n", strerror(ret));
    }
    printf("main thread: pid = %d   tid = %ld\n", getpid(), pthread_self());

    pthread_exit((void *)0);
    return 0;
}
                                                                                               

```



