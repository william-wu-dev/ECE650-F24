#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {

  char *argv[3];

  argv[0] = (char *)"ls";
  argv[1] = (char *)"-l";
  argv[2] = nullptr;

  // std::endl print line terminator and flushes output stream!
  std::cout << "[exec] executing '/bin/ls -l' using execv" << std::endl;

  pid_t kid;
  kid = fork();
  if (kid == 0) {
    std::cout << "[child] sleeping for 4 seconds.." << std::endl;
    sleep(4);
    execv("/bin/ls", argv);
    // execl("/bin/ls", "ls", "-l", nullptr);
    perror("Error from arie");
    return 1;
  }

  int res;
  std::cout << "[exec] waiting for child process to terminate" << std::endl;
  waitpid(kid, &res, 0);
  std::cout << "[exec] finished execution with return status: " << res
            << std::endl;

  return 0;
}
