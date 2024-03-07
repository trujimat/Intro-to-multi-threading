// Example showing the necessary use of mutex

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int result = 0;
pthread_mutex_t lock;

void *add_function()
{
    for (int i = 0; i < 10000000; i++)
    {
        pthread_mutex_lock(&lock);
        result++;
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
    pthread_mutex_init(&lock, NULL);
    pthread_t thr1, thr2;
    pthread_create(&thr1, NULL, add_function, NULL);
    pthread_create(&thr1, NULL, add_function, NULL);
    pthread_join(thr1, NULL);
    pthread_join(thr2, NULL);
    printf("The result is %d \n", result);
}