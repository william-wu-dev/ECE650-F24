#include <iostream>
#include <sstream>

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    std::exit(EXIT_FAILURE);                                                   \
  } while (0)

typedef struct values {
  int v1;
  int v2;
} values_t;

bool g_cancel;
pthread_mutex_t g_lock;

void *foo(void *data) {
  auto pid = getpid();
  auto tid = pthread_self();

  values_t *vals = static_cast<values_t *>(data);
  while (!g_cancel) {
    vals->v1++;
    std::stringstream out;
    out << "I am thread " << tid << " of process " << pid << std::endl;
    out << "\t vals.v1=" << vals->v1 << ", "
        << "vals.v2=" << vals->v2 << std::endl;
    std::cout << out.str() << std::flush;
  }
  return nullptr;
}

void *bar(void *data) {
  auto pid = getpid();
  auto tid = pthread_self();
  values_t *vals = static_cast<values_t *>(data);

  while (!g_cancel) {
    std::stringstream out;
    vals->v2++;
    out << "I am also a thread " << tid << " of process " << pid << std::endl;
    out << "\t vals.v1=" << vals->v1 << ", "
        << "vals.v2=" << vals->v2 << std::endl;
    std::cout << out.str() << std::flush;
  }

  return nullptr;
}

int main(void) {

  pthread_mutex_init(&g_lock, nullptr);
  values_t vals = {.v1 = 0, .v2 = 0};

  int ret = 0;
  pthread_t t1, t2;

  g_cancel = false;

  std::cout << "Running thread 1: " << std::endl;
  ret = pthread_create(&t1, nullptr, &foo, &vals);
  if (ret != 0)
    handle_error("create");
  std::cout << "Started thread: " << t1 << std::endl;

  std::cout << "Running thread 2: " << std::endl;
  ret = pthread_create(&t2, nullptr, &bar, &vals);
  if (ret != 0)
    handle_error("create");
  std::cout << "Started thread: " << t2 << std::endl;

  sleep(5);
  g_cancel = true;

  ret = pthread_join(t1, nullptr);
  if (ret != 0)
    handle_error("join");
  ret = pthread_join(t2, nullptr);
  if (ret != 0)
    handle_error("join");

  return 0;
}
