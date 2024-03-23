// This example shows the use of condition variables for multi threading

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t fuel_lock;
pthread_cond_t cond_fuel;
int fuel = 0;

void *fill_fuel()
{
    // Fills the fuel of a car 75 litres in steps of 15
    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&fuel_lock);
        fuel += 60;
        printf("Loding fuel, the fuel now is %d \n", fuel);
        pthread_mutex_unlock(&fuel_lock);
        pthread_cond_broadcast(&cond_fuel);
        sleep(1);
    }
}

void *car()
{
    // Drives the car an amount of distance that consumes 40 litres of fuel
    pthread_mutex_lock(&fuel_lock);
    while (fuel < 40)
    {
        printf("There is not enough fuel for the car to drive \n");
        pthread_cond_wait(&cond_fuel, &fuel_lock);
    }
    fuel -= 40;
    printf("The fuel now is %d \n", fuel);
    pthread_mutex_unlock(&fuel_lock);
}

int main()
{
    pthread_mutex_init(&fuel_lock, NULL);
    pthread_cond_init(&cond_fuel, NULL);
    pthread_t th[5];

    for (int i = 0; i < 5; i++)
    {
        if (i == 4)
        {
            if (pthread_create(&th[i], NULL, &fill_fuel, NULL) != 0)
            {
                perror("There was an error creating the fill fuel thread");
            }
        }
        else
        {
            if (pthread_create(&th[i], NULL, &car, NULL) != 0)
            {
                perror("There was an error creating the car thread");
            }
        }
    }

    for (int j = 0; j < 5; j++)
    {
        if (pthread_join(th[j], NULL) != 0)
        {
            perror("There was an error joining the thread");
        }
    }

    pthread_mutex_destroy(&fuel_lock);
    pthread_cond_destroy(&cond_fuel);
}