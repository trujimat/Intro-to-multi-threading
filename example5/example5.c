// This example shows the usage of barriers. We have 8 threads, each one throws a dice, the main thread informs which thread won

#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <bits/pthreadtypes.h>

#define NUM_THREADS 8

int dice_values[NUM_THREADS];
int status[NUM_THREADS] = {0};

pthread_barrier_t barrier_roll_dice, compute_status;

void *roll_dice(void *arg)
{
    int thr_index = *(int *)arg;
    dice_values[thr_index] = rand() % 6 + 1;

    pthread_barrier_wait(&barrier_roll_dice);
    pthread_barrier_wait(&compute_status);
    if (status[thr_index] == 1)
    {
        printf(" The thread %d rolled the number %d and won \n", thr_index, dice_values[thr_index]);
    }
    else
    {
        printf(" The thread %d rolled the number %d and lost \n", thr_index, dice_values[thr_index]);
    }
    free(arg);
}

int main()
{
    pthread_t th[NUM_THREADS];
    pthread_barrier_init(&barrier_roll_dice, NULL, NUM_THREADS + 1);
    pthread_barrier_init(&compute_status, NULL, NUM_THREADS + 1);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&th[i], NULL, &roll_dice, index) != 0)
        {
            perror("There was an error creating the thread \n");
        }
    }

    pthread_barrier_wait(&barrier_roll_dice);
    int max = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (dice_values[i] > max)
        {
            max = dice_values[i];
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        status[i] = (dice_values[i] == max) ? 1 : 0;
    }
    pthread_barrier_wait(&compute_status);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("There was an error joining the thread\n");
        }
    }

    pthread_barrier_destroy(&barrier_roll_dice);
    pthread_barrier_destroy(&compute_status);
}