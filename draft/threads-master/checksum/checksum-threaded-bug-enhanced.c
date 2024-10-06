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

int main() {
  list = NULL;
  char data[BLOCKS * BLOCK_SZ];

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

  pthread_exit(NULL);
}
