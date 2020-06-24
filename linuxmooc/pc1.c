#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 4
int buffer1[CAPACITY];
int buffer2[CAPACITY];
int in1, out1;
int in2, out2;


int buffer_is_empty(int flag)
{
    if (flag == 1) {
        return in1 == out1;
    } else {
        return in2 == out2;
    }
}


int buffer_is_full(int flag)
{
    if (flag == 1) {
        return (in1 + 1) % CAPACITY == out1;
    } else {
        return (in2 + 1) % CAPACITY == out2;
    }

}

int get_item(int flag)
{
    if (flag == 1) {
        int item;
        item = buffer1[out1];
        out1 = (out1 + 1) % CAPACITY;
        return item;
    } else {
        int item;
        item = buffer2[out2];
        out2 = (out2 + 1) % CAPACITY;
        return item;
    }


}

void put_item(int item, int flag)
{
    if (flag == 1) {
        buffer1[in1] = item;
        in1 = (in1 + 1) % CAPACITY;
    } else {
        buffer2[in2] = item;
        in2 = (in2 + 1) % CAPACITY;
    }

}



pthread_mutex_t mutex1, mutex2;
pthread_cond_t wait_empty_buffer1, wait_empty_buffer2;
pthread_cond_t wait_full_buffer1, wait_full_buffer2;

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&mutex2);
        while (buffer_is_empty(2))
            pthread_cond_wait(&wait_full_buffer2, &mutex2);

        item = get_item(2);
        printf("            consume item: %c\n", item);

        pthread_cond_signal(&wait_empty_buffer2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

void *calculate(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&mutex1);
        while (buffer_is_empty(1))
            pthread_cond_wait(&wait_full_buffer1, &mutex1);
        item = get_item(1);
        item -= 32;
        pthread_cond_signal(&wait_empty_buffer1);
        pthread_mutex_unlock(&mutex1);

        pthread_mutex_lock(&mutex2);
        while (buffer_is_full(2))
            pthread_cond_wait(&wait_empty_buffer2, &mutex2);
        printf("    calculate item: %c\n", item);
        put_item(item, 2);
        pthread_cond_signal(&wait_full_buffer2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

void *produce()
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&mutex1);
        while (buffer_is_full(1))
            pthread_cond_wait(&wait_empty_buffer1, &mutex1);

        item = 'a' + i;
        put_item(item, 1);
        printf("produce item: %c\n", item);
        pthread_cond_signal(&wait_full_buffer1);
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}

int main()
{
    pthread_t consumer_tid, calculate_tid;

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&wait_empty_buffer1, NULL);
    pthread_cond_init(&wait_full_buffer1, NULL);
    pthread_cond_init(&wait_empty_buffer2, NULL);
    pthread_cond_init(&wait_full_buffer2, NULL);
    //创建线程
    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&calculate_tid, NULL, calculate, NULL);

    produce();
    //等待线程
    pthread_join(consumer_tid, NULL);
    pthread_join(calculate_tid, NULL);
    return 0;
}
