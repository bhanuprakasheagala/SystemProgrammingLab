#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

// Function executed by each thread
void *thread_function(void *thread_id) {
    long tid = (long)thread_id;
    printf("Hello from thread %ld\n", tid);
    pthread_exit(NULL); // Exit the thread
}

int main() {
    pthread_t threads[NUM_THREADS]; // Array to store thread IDs
    int rc;
    long t;

    // Create threads
    for (t = 0; t < NUM_THREADS; t++) {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)t);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Wait for threads to finish
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_join(threads[t], NULL);
        if (rc) {
            printf("ERROR: return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("Thread %ld joined\n", t);
    }

    printf("Main thread exiting\n");
    pthread_exit(NULL); // Exit the main thread
}
