// In this example we show the usage of multithreading in for loops and how to pass arguments to a thread

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;
int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
int total_sum = 0;

void *add_routine(void *arg)
{
    int index = *(int *)arg;
    for (int i = 0; i < 5; i++)
    {
        total_sum += primes[index * 5 + i];
    }
    free(arg);
}

int main()
{
    pthread_mutex_init(&lock, NULL);
    pthread_t th[2];

    for (int i = 0; i < 2; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&th[i], NULL, *add_routine, index) != 0)
        {
            perror("There was an error creating the thread");
        }
    }

    for (int j = 0; j < 2; j++)
    {
        if (pthread_join(th[j], NULL) != 0)
        {
            perror("There was an error joining the thread");
        }
    }

    printf("The total sum is %d \n", total_sum);
}