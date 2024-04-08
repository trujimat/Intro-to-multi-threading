// This example shows the usage of semaphores in the producer-consumer problem

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 8

pthread_mutex_t buffer_mutex;
sem_t full;
sem_t empty;
int buffer[10];
int buffer_index = 0;

void *consumer()
{
    while (1)
    {
        int shower_variable = -1;
        sem_wait(&full);
        pthread_mutex_lock(&buffer_mutex);
        shower_variable = buffer[buffer_index - 1];
        buffer_index--;
        printf("The value of the buffer in consumer is %d \n", shower_variable);
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty);
    }
}

void *producer()
{
    while (1)
    {
        sem_wait(&empty);
        sleep(1);
        pthread_mutex_lock(&buffer_mutex);
        buffer[buffer_index] = rand() % 100;
        buffer_index++;
        printf("The value of the buffer in producer is %d \n", buffer[buffer_index - 1]);
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full);
    }
}

int main()
{
    pthread_t th[8];
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, 10);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (i < 5)
        {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0)
            {
                perror("There was an error creating the producer thread");
            }
        }
        else
        {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0)
            {
                perror("There was an error creating the consumer thread");
            }
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("There was an error joining the thread");
        }
    }

    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 1;
}