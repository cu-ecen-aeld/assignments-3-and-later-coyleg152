#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
// #define DEBUG_LOG(msg,...)
// #define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
// #define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    /**
     * DONE: wait, obtain mutex, wait, release mutex
     * as described by thread_data structure
     *
     * hint: use a cast like the one below to obtain
     * thread arguments from your parameter
     *
     * struct thread_data* thread_func_args = (struct thread_data *) thread_param;
     */
    struct thread_data* func_args = (struct thread_data *) thread_param;

    printf("Obtain wait time = %dms\n", func_args->obtain_wait);
    usleep((func_args->obtain_wait) * 1000);

    int rc = pthread_mutex_lock(func_args->thread_mutex);
    if (rc != 0) {
        func_args->thread_complete_success = false;
        return thread_param;
    }

    printf("Release wait time = %dms\n", func_args->release_wait);
    usleep((func_args->release_wait) * 1000);

    rc = pthread_mutex_unlock(func_args->thread_mutex);
    if (rc != 0) func_args->thread_complete_success = false;

    return thread_param;
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,
    int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * DONE: allocate memory for thread_data, setup mutex and wait arguments,
     * pass thread_data to created thread using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data * thread_param = malloc(sizeof(struct thread_data));
    thread_param->thread_mutex = mutex;
    thread_param->obtain_wait = wait_to_obtain_ms;
    thread_param->release_wait = wait_to_release_ms;
    thread_param->thread_complete_success = true;

    int rc = pthread_create(thread, NULL, threadfunc, thread_param);
    if (rc != 0) {
        fprintf(stderr, "ERROR: pthread_create() failed\n");
        return false;
    }

    return true;
}

// Comment or delete this when done
// int main() {return 0;}
