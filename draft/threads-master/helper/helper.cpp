#include "helper.hpp"

#include <iostream>
#include <cstdlib>

#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

/*
 * Helper routines for pthreads
 */

void pclock(char *msg, clockid_t cid) {
  struct timespec ts;
  std::cout << msg << std::flush;
  if (clock_gettime(cid, &ts) == -1) {
    perror("clock_gettime");
    std::exit(0);
  }

  printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);
}

void errp(char *s, int code) {
  std::cerr << "Error: " << s << " -- " << strerror(code) << std::endl;
}

void thr_sleep(time_t sec, long nsec) {
  struct timeval now;
  struct timezone tz;
  struct timespec ts;
  int retcode;

  pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

  retcode = pthread_mutex_lock(&m);
  if (retcode) {
    std::cerr << "Error: mutex_lock" << std::endl;
    std::exit(0);
  }

  gettimeofday(&now, &tz);
  ts.tv_sec = now.tv_sec + sec + (nsec / 1000000000L);
  ts.tv_nsec = now.tv_usec * 1000 + (nsec % 1000000000L);
  if (ts.tv_nsec > 1000000000L) {
    (ts.tv_sec)++;
    (ts.tv_nsec) -= 1000000000L;
  }

  retcode = pthread_cond_timedwait(&cond, &m, &ts);

  if (retcode != ETIMEDOUT) {
    if (retcode == 0) {
      std::cerr <<  "pthread_cond_timedwait, (zero)" << std::endl;
    } else {
      std::cerr <<  "pthread_cond_timedwait, " << strerror(retcode) << std::endl;
      std::exit(0);
    }
  }

  retcode = pthread_mutex_unlock(&m);
  if (retcode) {
    std::cerr <<  "mutex_unlock, " << strerror(retcode) << std::endl;
    std::exit(0);
  }

  retcode = pthread_cond_destroy(&cond);
  if (retcode) {
    std::cerr <<  "mutex_cond_destroy, " << strerror(retcode) << std::endl;
    fflush(stdout);
  }

  retcode = pthread_mutex_destroy(&m);
  if (retcode) {
    std::cerr <<  "mutex_destroy, " << strerror(retcode) << std::endl;
    std::exit(0);
  }
}

void mulock(int ul, pthread_mutex_t *m) {
  int retcode = 0;
  std::string errorMsg;

  if (ul) {
    /* unlock */
    errorMsg = "mutex_unlock";
    retcode = pthread_mutex_unlock(m);
  } else {
    /* lock */
    errorMsg = "mutex_lock";
    retcode = pthread_mutex_lock(m);
  }

  if (retcode) {
    std::cerr << errorMsg << ", " << strerror(retcode) << std::endl;
    std::exit(0);
  }
}
