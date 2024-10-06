#pragma once

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

enum {
  LOCK,
  UNLOCK,
};

extern void errp(char *s, int code);
extern void thr_sleep(time_t sec, long nsec);
extern void mulock(int ul, pthread_mutex_t *m);
extern void pclock(char *msg, clockid_t cid);
