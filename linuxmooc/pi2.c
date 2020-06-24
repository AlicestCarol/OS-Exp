
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NR_TOTAL 100000
#define NR_CPU 4
#define NR_CHILD (NR_TOTAL/NR_CPU)

struct param {
    int start;
    int end;
};

struct result {
    float sum;
};

void *compute(void *arg)
{
    struct param *param;
    struct result *result;
    float sum = 0;
    int i;
    int flag=1;
    param = (struct param *)arg;
    for (i = 2*param->start+1; i < 2*param->end+1; i+=2)
    {
        sum += 1.0/i*flag;
        flag=-flag;
    }    
    result = malloc(sizeof(struct result));
    result->sum = sum;
    return result;
}

int main()
{ 
    pthread_t workers[NR_CPU];
    struct param params[NR_CPU]; 
    int i;
    //使用线程参数
    for (i = 0; i < NR_CPU; i++) {
        struct param *param;
        param = &params[i];
        param->start = i * NR_CHILD; 
        param->end = (i + 1) * NR_CHILD;
        pthread_create(&workers[i], NULL, compute, param);
    }

    float PI = 0;
    for (i = 0; i < NR_CPU; i++) {
        struct result *result;
        pthread_join(workers[i], (void **)&result);
        PI += result->sum;
        free(result);
    }
    PI*=4;

    printf("PI = %f\n", PI);
    return 0;
}