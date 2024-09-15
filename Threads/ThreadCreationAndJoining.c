/*
Create two threads that execute a simple function and print messages
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Function to be executed by threads
void* thread_function(void* arg) {
    int thread_id = *((int*) arg);
    printf("Thread %d is running\n", thread_id);
    return NULL;
}

int main() {

    pthread_t thread1, thread2; // Thread identifiers
    int thread1_id = 1, thread2_id = 2;

    // Create two threads
    if(pthread_create(&thread1, NULL, thread_function, &thread1_id) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }
    if(pthread_create(&thread2, NULL, thread_function, &thread2_id) != 0) {
        perror("Failed to create thread 2");
        return 1;
    }

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Both threads have completed execution\n");

    return 0;
}