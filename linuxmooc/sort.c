#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int array[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

#define NR_TOTAL 10
#define NR_CPU 2
#define NR_CHILD (NR_TOTAL/NR_CPU)

struct param {
    int *array;
    int start;
    int end;
};


// 选择排序
void *selectSort(void *arg)
{
    struct param *param;
    param = (struct param *) arg;
    int i, j, k;
    for (i = param->start; i < param->end - 1; i++) {
        k = i;
        for (j = i + 1; j < param->end; j++) {
            if (param->array[k] > param->array[j])
                k = j;
        }
        if (k != i) {
            param->array[k] = param->array[k] ^ param->array[i];
            param->array[i] = param->array[k] ^ param->array[i];
            param->array[k] = param->array[k] ^ param->array[i];
        }
    }
    return NULL;
}

//二路归并
void merge(int data[],int first,int mid,int last)
{
    int temp[last-first+1],m;
    for(m=first;m<=last;m++)
    {
        temp[m-first]=data[m];
    }
    
    int i=first,j=mid+1,k=first;

    while(i<=mid&&j<=last)
    {
        if(temp[i-first]<=temp[j-first])
        {
            data[k++]=temp[i-first];
            i++;    
        }
        else
        {
            data[k++]=temp[j-first];
            j++;
        }
    }

    while(i<=mid)
        {
            data[k++]=temp[i-first];
            i++;   
        }
    while(j<=last)
        {
            data[k++]=temp[j-first];
            j++;
        }
        
}


//归并排序
int MergeSort(int data[])
{
    int i,k;
	for(k=1;k<=NR_TOTAL;k*=2)
    {
        for(i=0;i+k<NR_TOTAL;i+=2*k)
        {
            merge(data,i,i+k-1,i+2*k-1<NR_TOTAL-1?i+2*k-1:NR_TOTAL-1);
        } 
    } 
    
} 


void PrintArray(int i)
{
    if (i == 0)
        printf("  原数组: ");
    else
        printf("有序数组: ");

    for (i = 0; i < NR_TOTAL; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main()
{
    pthread_t workers[NR_CPU];
    struct param params[NR_CPU];
    int i;
    PrintArray(0);
    //通过选择排序算法，对两部分排序
    for (i = 0; i < NR_CPU; i++) {
        struct param *param;
        param = &params[i];
        param->array = array;
        param->start = i * NR_CHILD;
        param->end = (i + 1) * NR_CHILD;
        pthread_create(&workers[i], NULL, selectSort, param);
    }

    //等待子线程排序完成
    for (i = 0; i < NR_CPU; i++) {
        pthread_join(workers[i], NULL);
    }

    //调用归并排序
    MergeSort(array);
    
    PrintArray(1);

    return 0;
}
