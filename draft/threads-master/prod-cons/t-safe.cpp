#include "helper.hpp"

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

/*
 *
 * The producer-consumer problem using pthreads. This one is NOT safe.
 * A context-switch within the critical section causes serious problems.
 * We place sched_yield() calls within cons() to increase the chances that
 * a problem occurs.
 */

#define BUF_SZ 10
#define VAL_MIN 1
#define VAL_MAX 100

typedef struct pc_typ {
  unsigned int head, tail;
  unsigned int buf[BUF_SZ];
  pthread_mutex_t lk;
} pc_typ;

void *prod(void *arg) {
  auto *d = static_cast<pc_typ *>(arg);

  while (1) {
    unsigned int h, t;

    mulock(LOCK, &(d->lk));
    h = d->head;
    t = d->tail;
    mulock(UNLOCK, &(d->lk));

    if (((h + 1) % BUF_SZ) == t) {
      /* buf[] is full */
      continue;
    }

    int val = (rand() % (VAL_MAX - VAL_MIN + 1)) + VAL_MIN;
    std::stringstream sout;
    sout << "prd: buf[" << h << "] <-- " << val << std::endl;
    std::cout << sout.str() << std::flush;

    mulock(LOCK, &(d->lk));
    (d->buf)[(d->head)++] = val;

    if ((d->head) >= BUF_SZ) {
      (d->head) %= BUF_SZ;
    }
    mulock(UNLOCK, &(d->lk));
  }
}

void *cons(void *arg) {
  auto *d = static_cast<pc_typ *>(arg);

  while (1) {
    unsigned int h, t;

    mulock(LOCK, &(d->lk));
    h = d->head;
    t = d->tail;
    mulock(UNLOCK, &(d->lk));

    if (h == t) {
      /* buf[] is empty */
      continue;
    }

    int val = (d->buf)[t];
    std::stringstream sout;
    sout << "cons: buf[" << t << "] == " << val << std::endl;
    std::cout << sout.str() << std::flush;

    mulock(LOCK, &(d->lk));
    (d->tail)++;

    sched_yield();
    sched_yield();
    sched_yield();

    if ((d->tail) >= BUF_SZ) {
      (d->tail) %= BUF_SZ;
    }
    mulock(UNLOCK, &(d->lk));
  }
}

int main() {
  pthread_t tc, tp;
  pc_typ d;
  int retcode = 0;

  bzero(&d, sizeof(pc_typ));
  srand(getpid());

  retcode = pthread_mutex_init(&(d.lk), nullptr);
  if (retcode) {
    std::cerr << "mutex_init(cons) " << strerror(retcode) << std::endl;
    return 0;
  }

  retcode = pthread_create(&tc, nullptr, &cons, (void *)&d);
  if (retcode) {
    std::cerr <<"pthread_create(cons) " << strerror(retcode) << std::endl;
    return 0;
  }

  retcode = pthread_create(&tp, nullptr, &prod, (void *)&d);
  if (retcode) {
    std::cerr <<"pthread_create(prod) " << strerror(retcode) << std::endl;
    return 0;
  }

  retcode = pthread_join(tc, nullptr);
  if (retcode) {
    std::cerr <<"pthread_join(cons) " << strerror(retcode) << std::endl;
    return 0;
  }

  retcode = pthread_join(tp, nullptr);
  if (retcode) {
    std::cerr <<"pthread_join(prod) " << strerror(retcode) << std::endl;
    return 0;
  }

  return 0;
}
