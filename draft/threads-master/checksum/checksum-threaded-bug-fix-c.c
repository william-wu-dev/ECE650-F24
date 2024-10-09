#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>

struct Node {
  uint64_t chksum;
  struct Node *next;
};

struct Node *list;
void *thread_checksum(void *);

#define BLOCKS 4
#define BLOCK_SZ 1024*1024

pthread_mutex_t modify_linked_list;
/*
This is a lock called mutex. pthread_mutex_t is in header <pthread.h>
I found a useful guide about mutex here: https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/
*/

int main() {
  list = NULL;
  char data[BLOCKS * BLOCK_SZ];

  if (pthread_mutex_init(&modify_linked_list, NULL) != 0) { 
    /*
    the mutex initialization, use address of the mutex as input

    int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr) : 
    
    Creates a mutex, referenced by mutex, with attributes specified by attr. If attr is NULL, the default mutex attribute (NONRECURSIVE) is used.

    Returned value
    If successful, pthread_mutex_init() returns 0, and the state of the mutex becomes initialized and unlocked.
    If unsuccessful, pthread_mutex_init() returns -1.
    */
        perror("mutex init has failed"); 
        return 1; 
    } 

  FILE *file = fopen("data.bin", "r");
  if (file == NULL) {
    perror("Could not open file\n");
    return 1;
  }

  size_t sz;
  sz = fread(data, 1, sizeof(data), file);
  if (sz < sizeof(data)) {
    perror("Could not read");
    exit(1);
  }

  // creates and starts a thread --- see man pthread_create for details
  pthread_t thread1;
  pthread_create(&thread1, NULL, thread_checksum, data);

  pthread_t thread2;
  pthread_create(&thread2, NULL, thread_checksum, data + 1024 * 1024);

  pthread_t thread3;
  pthread_create(&thread3, NULL, thread_checksum, data + 2 * 1024 * 1024);

  pthread_t thread4;
  pthread_create(&thread4, NULL, thread_checksum, data + 3 * 1024 * 1024);

  // Now wait for threads to complete execution before using their partial
  // results
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  pthread_join(thread4, NULL);
  // regardless of the order in which they completed (which there is no way
  // to determine with certainty), we do know that at this point ALL of them
  // finished --- when we call pthread_join, if the given thread has not
  // finished, then the call blocks the calling thread (the main thread, in
  // this case).  If the thread has completed, then the call returns
  // immediately, without blocking the thread.

  /* when multi-threading things are done, destroy mutex */
  pthread_mutex_destroy(&modify_linked_list);
  /*
  int pthread_mutex_destroy(pthread_mutex_t *mutex) : 
  
  Deletes a mutex object, which identifies a mutex. Mutexes are used to protect shared resources. mutex is set to an invalid value, but can be reinitialized using pthread_mutex_init().

  Returned value
  If successful, pthread_mutex_destroy() returns 0.
  If unsuccessful, pthread_mutex_destroy() returns -1.
  */

  uint64_t chksum = 0;
  struct Node *elem = list;
  while (elem != NULL) {
    chksum ^= elem->chksum;
    elem = elem->next;
  }

  printf("Checksum: %" PRIx64 "\n", chksum);
}

void *thread_checksum(void *bytes) {
  uint64_t chksum = 0;

  uint64_t *data = bytes;
  int i;

  for (i = 0; i < BLOCK_SZ / 8; ++i) {
    int k;
    uint64_t tmp = *data++;
    for (k = 0; k < 1024; ++k) {
      chksum *= tmp++;
      chksum += tmp++;
      chksum *= tmp++;
      chksum ^= tmp++;
      chksum *= tmp++;
    }
  }

  /* lock mutex before the critical section */
  pthread_mutex_lock(&modify_linked_list);
  /*
  int pthread_mutex_lock(pthread_mutex_t *mutex) : 
  
  Locks a mutex object, which identifies a mutex. If the mutex is already locked by another thread, the thread waits for the mutex to become available. The thread that has locked a mutex becomes its current owner and remains the owner until the same thread has unlocked it. When the mutex has the attribute of recursive, the use of the lock may be different. When this kind of mutex is locked multiple times by the same thread, then a count is incremented and no waiting thread is posted. The owning thread must call pthread_mutex_unlock() the same number of times to decrement the count to zero.

  Returned value
  If successful, pthread_mutex_lock() returns 0.
  If unsuccessful, pthread_mutex_lock() returns -1.
  */

  if (list == NULL) {
    usleep(100000);
    list = malloc(sizeof(struct Node));
    usleep(100000);
    list->chksum = chksum;
    usleep(100000);
    list->next = NULL;
  } else {
    struct Node *tmp = list;
    usleep(100000);
    list = malloc(sizeof(struct Node));
    usleep(100000);
    list->chksum = chksum;
    usleep(100000);
    list->next = tmp;
  }

  /* end of critical section, release mutex */
  pthread_mutex_unlock(&modify_linked_list);
  /*
  int pthread_mutex_unlock(pthread_mutex_t *mutex) : 
  
  Releases a mutex object. If one or more threads are waiting to lock the mutex, pthread_mutex_unlock() causes one of those threads to return from pthread_mutex_lock() with the mutex object acquired. If no threads are waiting for the mutex, the mutex unlocks with no current owner. When the mutex has the attribute of recursive the use of the lock may be different. When this kind of mutex is locked multiple times by the same thread, then unlock will decrement the count and no waiting thread is posted to continue running with the lock. If the count is decremented to zero, then the mutex is released and if any thread is waiting for it is posted.
  
  Returned value
  If successful, pthread_mutex_unlock() returns 0.
  If unsuccessful, pthread_mutex_unlock() returns -1
  */

  pthread_exit(NULL);
}
