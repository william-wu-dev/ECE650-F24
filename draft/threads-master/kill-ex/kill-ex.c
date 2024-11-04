#include "helper.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

/*
 *
 * Example of pthread_kill() to check whether a thread is still alive,
 * and pthread_cancel() to kill it.
 */

void *p(void *arg) {
  printf("Thread %u up and running.\n", pthread_self());
  fflush(stdout);
  // consuming some cpu time
  for (unsigned int j = 0; j < 2000000; j++)
               getppid();
  // thr_sleep(1, 0);
  sleep(10); //Suspends thread execution for a specified number of seconds.
  // https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-sleep-suspend-execution-thread
  printf("Thread %u exiting.\n", pthread_self());
  fflush(stdout);
}

int main() {
  pthread_t t;
  int ret;

  pthread_create(&t, NULL, &p, (void *)NULL);

  // thr_sleep(0, 500000000);
  sleep(1);

  if (ret = pthread_kill(t, 0)) {
    printf("main thread: kill() 1 returned error: %s\n", strerror(ret));
    fflush(stdout);
  } else {
    printf("main thread: child running well.\n");
    fflush(stdout);
  }

  // thr_sleep(0, 250000000);
  sleep(2);

  if (ret = pthread_kill(t, 0)) {
    printf("main thread: kill() 2 returned error: %s\n", strerror(ret));
    fflush(stdout);
  } else {
    ret = pthread_cancel(t);
    if (!ret) {
      printf("main thread: child thread cancelled successfully.\n");
      fflush(stdout);
    } else {
      printf("main thread: cancel returned error: %s\n", strerror(ret));
      fflush(stdout);
    }
  }

  if (ret = pthread_kill(t, 0)) {
    printf("main thread: kill() 3 returned error: %s\n", strerror(ret));
    fflush(stdout);
  }

  clockid_t cid;
  if (ret = pthread_getcpuclockid(t, &cid)) {
    printf("main thread: pthread_getcpuclockid() returned error: %s\n", strerror(ret));
    fflush(stdout);
  } else {
    // extract timespec out of cid
    struct timespec ts;
    if (clock_gettime(cid, &ts) == -1) {
      printf("main thread: clock_gettime() returned error: %s\n", strerror(ret));
    }
    printf("%4jd.%03ld\n", (intmax_t) ts.tv_sec, ts.tv_nsec / 1000000);
  }

  return 0;
}
