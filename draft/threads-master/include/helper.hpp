#pragma once

#include <time.h>
#include <pthread.h>

enum {
  LOCK,
  UNLOCK,
};

extern void errp(char *s, int code);
extern void thr_sleep(time_t sec, long nsec);
extern void mulock(int ul, pthread_mutex_t *m);
extern void pclock(char *msg, clockid_t cid);
