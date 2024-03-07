// A simple example to showcasing threads

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *example_function()
{
    printf("Entered the function \n");
    sleep(2);
    printf("About to exit the function \n");
}

int main()
{
    pthread_t thr1, thr2;
    pthread_create(&thr1, NULL, example_function, NULL);
    pthread_create(&thr2, NULL, example_function, NULL);
    pthread_join(thr1, NULL);
    pthread_join(thr2, NULL);
}