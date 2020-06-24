
#include <stdio.h>
#include <pthread.h>

#define NUMBER  100000

float worker_output;

void *worker(void *arg)
{
    int i;
    int flag=1;
    for (i = 1; i < 2 *(NUMBER / 2)+1; i+=2)
    {
        worker_output += 1.0/i*flag;
        flag=-flag;
    }    
    return NULL;
}

float master_output;

void master()
{
    int i;
    int flag=1;
    for (i = 2*(NUMBER / 2)+1; i < 2*NUMBER+1; i+=2)
    {
        worker_output += 1.0/i*flag;
        flag=-flag;
    }    
}


int main()
{ 
    pthread_t worker_tid;
    float PI;
    pthread_create(&worker_tid, NULL, worker, NULL);
    master(); 
    //等待子线程
    pthread_join(worker_tid, NULL);

    PI = (master_output + worker_output)*4;
    printf("PI = %f\n",PI);
    return 0;
}