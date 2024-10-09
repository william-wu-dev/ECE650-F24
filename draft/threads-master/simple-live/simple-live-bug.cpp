#include <iostream>
// #include <pthread.h>
// #include <stdio.h> // perror is defined in header <stdio.h>
// #include <sys/types.h>
#include <unistd.h>

void *foo(void *data)
{
    auto pid = getpid();
    auto tid = pthread_self();

    int *vals = static_cast<int *>(data);
    for (int i = 0; i < 10000; i++)
    {
        *vals++;

        std::cout << "I am thread " << tid << " of process " << pid << std::endl;
        std::cout << "\t value= " << *vals << std::endl;
    }
    return nullptr;
}

void *bar(void *data)
{
    auto pid = getpid();
    auto tid = pthread_self();
    int *vals = static_cast<int *>(data);

    for (int i = 0; i < 10000; i++)
    {
        *vals++;

        std::cout << "I am also a thread " << tid << " of process " << pid << std::endl;
        std::cout << "\t value=" << *vals << std::endl;
    }
    return nullptr;
}

int main(void)
{

    int vals = 0;

    int ret = 0;

    pthread_t t1, t2;

    std::cout << "Create and run thread 1: " << std::endl;
    ret = pthread_create(&t1, nullptr, &foo, &vals);
    if (ret != 0)
    {
        perror("create thread 1 error");
        return 1;
    }
    std::cout << "Started thread: " << t1 << std::endl;

    std::cout << "Create and run thread 2: " << std::endl;
    ret = pthread_create(&t2, nullptr, &bar, &vals);
    if (ret != 0)
    {
        perror("create thread 2 error");
        return 1;
    }
    std::cout << "Started thread: " << t2 << std::endl;

    
    ret = pthread_join(t1, nullptr);
    if (ret != 0)
    {
        perror("join thread 1 error");
        return 1;
    }
    ret = pthread_join(t2, nullptr);
    if (ret != 0)
    {
        perror("join thread 2 error");
        return 1;
    }

    return 0;
}

/*
1. pthread_create: used to create a new thread
Syntax:
int pthread_create(pthread_t * thread,
                   const pthread_attr_t * attr,
                   void * (*start_routine)(void *),
                   void *arg);

Parameters:
  • thread: pointer to an unsigned integer value that returns the thread id of the thread created.
  • attr: pointer to a structure that is used to define thread attributes like detached state, scheduling policy, stack address, etc. Set to NULL for default thread attributes.
  • start_routine: pointer to a subroutine that is executed by the thread. The return type and parameter type of the subroutine must be of type void *. The function has a single attribute but if multiple values need to be passed to the function, a struct must be used.
  • arg: pointer to void that contains the arguments to the function defined in the earlier argument
Return: On success, pthread_create() returns 0; on error, it returns an error number, and the contents of *thread are undefined.

2. pthread_join: used to wait for the termination of a thread.
Syntax:
int pthread_join(pthread_t th,
                 void **thread_return);

Parameter: This method accepts following parameters:
  • th: thread id of the thread for which the current thread waits.
  • thread_return: pointer to the location where the exit status of the thread mentioned in th is stored.
Return: If successful, the pthread_join() function shall return zero; otherwise, an error number shall be returned to indicate the error.

Clipped from: https://www.geeksforgeeks.org/thread-functions-in-c-c/
*/
