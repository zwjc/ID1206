#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define RESET "\033[0m"
#define GREEN "\033[32m"

int buffer = 0;
pthread_mutex_t mutex;

void *increment()
{
    // Have to allocate memory since otherwise if we just create a normal int in this function it wont be reached from the main function (since it would be on the stack)
    int *timesModified = malloc(sizeof(int));
    while (1)
    {
        // Basically does so only one thread can run whats inside the lock and unlock mutex at a time
        pthread_mutex_lock(&mutex);
        if (buffer < 15)
        {
            printf("TID: %lu, PID: %d, Buffer: %d\n", pthread_self(), getpid(), buffer++);
            *timesModified = *timesModified + 1;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            return (void *)timesModified;
        }
    }
}

int main()
{
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Initialize ints that will be used to store the return values from the threads
    int *resOne;
    int *resTwo;
    int *resThree;

    // Create 3 thread ids
    pthread_t threadIdOne;
    pthread_t threadIdTwo;
    pthread_t threadIdThree;

    // Create 3 threads that run the increment function
    if (pthread_create(&threadIdOne, NULL, &increment, NULL) != 0)
    {
        // Some error has occured
        return 1;
    }
    if (pthread_create(&threadIdTwo, NULL, &increment, NULL) != 0)
    {
        // Some error has occured
        return 2;
    }
    if (pthread_create(&threadIdThree, NULL, &increment, NULL) != 0)
    {
        // Some error has occured
        return 3;
    }

    // Create 3 ints to store the number of times each thred modified the buffer
    int oneModifiedAmount;
    int twoModifiedAmount;
    int threeModifiedAmount;

    // Get return value from the threads
    if (pthread_join(threadIdOne, (void **)&resOne) != 0)
    {
        // Some error has occured
        return 4;
    }
    if (pthread_join(threadIdTwo, (void **)&resTwo) != 0)
    {
        // Some error has occured
        return 5;
    }
    if (pthread_join(threadIdThree, (void **)&resThree) != 0)
    {
        // Some error has occured
        return 6;
    }

    // Print out in the same way as described in the assignment
    printf("TID: %lu worked on the buffer %d ", threadIdOne, *resOne);
    printf(GREEN "times\n" RESET);
    printf("TID: %lu worked on the buffer %d ", threadIdTwo, *resTwo);
    printf(GREEN "times\n" RESET);
    printf("TID: %lu worked on the buffer %d ", threadIdThree, *resThree);
    printf(GREEN "times\n" RESET);
    printf("Total buffer accesses: %d\n", buffer);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    // Since we are using malloc we need to deallocate the memory
    free(resOne);
    free(resTwo);
    free(resThree);

    return 0;
}