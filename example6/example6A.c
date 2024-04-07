// In this example is showed the use of semaphores, the example ilustrates the usage of a login queue where 8 threads can share resources at the same time

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 12
sem_t queue_semaphore;

void* routine(void* arg){
    printf("(%d) Waiting on the queue to join the server \n", *(int *)arg);
    sem_wait(&queue_semaphore);
    printf("(%d) joined the server \n", *(int *)arg);
    sleep(rand()%5 +1);
    printf("(%d) abandoned the server \n", *(int *)arg);
    sem_post(&queue_semaphore);

}

int main(){
    sem_init(&queue_semaphore, 0, 8);
    pthread_t th[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        int *index = malloc(sizeof(int));
        *index = i;
        if (pthread_create(&th[i], NULL, &routine, index) != 0)
        {
            perror("There was an error creating the thread!");
        }
        
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("There was an error joining the thread!");
        }
    }
    
    sem_destroy(&queue_semaphore);
}