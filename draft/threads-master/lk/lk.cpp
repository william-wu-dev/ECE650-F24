#include "helper.hpp"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
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
 * An example of using mutex that is shared by multiple threads.
 * We have three reader threads that all synchronize on the mutex, that is
 * initially locked by the main thread.
 *
 * Then, the main thread creates a writer thread one by one, each of which
 * writes a value to the global variable i, and then unlocks the mutex.
 *
 * NOTE: this is NOT a program that shows effective use of mutexes. Various
 * bad things can and do happen when we run this program. For example, a reader
 * may print out 0 when we expect it to print 10, 20 or 30 (which are the values
 * that the writers write). Another problem is that a reader can starve while
 * blocked on the lock(mutex) call. This happens when two writers unlock the
 * mutex before a reader is able to lock it. The second unlock, therefore, has
 * no effect, and a reader starves.
 */

int i;
pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;

void *wrt(void *arg) {
  int *val = (int *)arg;

  i = *val;
  std::cout << "Writer: about to unlock" << std::endl;
  mulock(UNLOCK, &l);
  return nullptr;
}

void *rd(void *arg) {
  int *rid = (int *)arg;

  std::stringstream sout;
  sout << "Reader " << *rid << " locking" << std::endl;
  std::cout << sout.str() << std::flush;

  mulock(LOCK, &l);

  sout.clear();
  sout << "Reader " << *rid << " read i = " << i << std::endl;
  std::cout << sout.str() << std::flush;

  i = 0;
  return nullptr;
}

int main() {
  i = 0;

  pthread_t w, r[3];
  int rid[3], wval;

  mulock(LOCK, &l);

  rid[0] = 1;
  pthread_create(&(r[0]), nullptr, &rd, (void *)&(rid[0]));

  rid[1] = 2;
  pthread_create(&(r[1]), nullptr, &rd, (void *)&(rid[1]));

  rid[2] = 3;
  pthread_create(&(r[2]), nullptr, &rd, (void *)&(rid[2]));

  wval = 10;
  pthread_create(&w, nullptr, &wrt, (void *)&wval);

  pthread_join(w, nullptr);

  wval = 20;
  pthread_create(&w, nullptr, &wrt, (void *)&wval);

  pthread_join(w, nullptr);

  wval = 30;
  pthread_create(&w, nullptr, &wrt, (void *)&wval);

  pthread_join(w, nullptr);
  pthread_join(r[0], nullptr);
  pthread_join(r[1], nullptr);

  return 0;
}
