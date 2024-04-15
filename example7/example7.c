/* This example shows the usage of pools of threads to compare velocity between single threaded programs and multi threaded 
programs. Modifying the value of NUM_THREADS to 1 it can be appreciated that the program goes significantly slower*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 8

pthread_cond_t task_available;
pthread_mutex_t task_queue_mutex;
int task_count = 0;

typedef struct Task
{
    void (*operation)(int, int);
    int a;
    int b;
} Task;

Task task_queue[256];

void sum(int a, int b)
{
    usleep(10000);
    int result = a + b;
    printf("The result of the sum between %d and %d is %d \n", a, b, result);
}

void product(int a, int b)
{
    usleep(10000);
    int result = a * b;
    printf("The result of the multiplication between %d and %d is %d \n", a, b, result);
}

void execute(Task *task)
{
    task->operation(task->a, task->b);
}

void enquee(Task *task)
{
    pthread_mutex_lock(&task_queue_mutex);
    task_queue[task_count] = *task;
    task_count++;
    pthread_mutex_unlock(&task_queue_mutex);
    pthread_cond_signal(&task_available);
}

void *start()
{
    while (1)
    {
        pthread_mutex_lock(&task_queue_mutex);
        while (task_count == 0)
        {
            pthread_cond_wait(&task_available, &task_queue_mutex);
        }
        task_count--;
        Task task = task_queue[task_count];
        pthread_mutex_unlock(&task_queue_mutex);
        execute(&task);
    }
}

int main()
{
    pthread_t th[NUM_THREADS];
    pthread_cond_init(&task_available, NULL);
    pthread_mutex_init(&task_queue_mutex, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(&th[i], NULL, &start, NULL) != 0)
        {
            perror("There was an error creating the thread");
        }
    }

    for (int i = 0; i < 100; i++)
    {
        Task task = {
            .operation = i % 2 == 0 ? &sum : &product,
            .a = rand() % 100,
            .b = rand() % 100};
        enquee(&task);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("There was an error while joining the thread");
        }
    }
    pthread_cond_destroy(&task_available);
    pthread_mutex_destroy(&task_queue_mutex);
}