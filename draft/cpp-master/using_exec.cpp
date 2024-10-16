#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <cerrno>

int main(void) {

  char *argv[3];

  argv[0] = (char *)"ls";
  argv[1] = (char *)"-l";
  // argv[2] = nullptr;
  argv[2] = (char *)"not_exist";

  // std::endl print line terminator and flushes output stream!
  std::cout << "[exec] executing '/bin/ls -l' using execv" << std::endl;

  pid_t kid;
  kid = fork();
  if (kid == 0) {
    std::cout << "[child] sleeping for 4 seconds.." << std::endl;
    sleep(4);
    execv("/bin/ls", argv);
    // execl("/bin/ls", "ls", "-l", nullptr);
    perror("Error from arie");  // the equivalent is in the follow
    // Clipped from: https://www.ibm.com/docs/en/i/7.3?topic=functions-perror-print-error-message
    // perror() — Print Error Message
    // Defined in header <cstdio>
    // If string is not NULL and does not point to a null character, the string pointed to by string is printed to the standard error stream, followed by a colon and a space.
    // The message associated with the value in errno is then printed followed by a new-line character.
    std::cerr << "Error Message by strerror: " << strerror(errno) << std::endl;
    // strerror in <cstring> and errno in <cerrno>
    // Returns a pointer to the textual description of the system error code errnum, identical to the description that would be printed by std::perror().
    // Clipped from: https://en.cppreference.com/w/cpp/string/byte/strerror
    return 1;
  }

  int res;
  std::cout << "[exec] waiting for child process to terminate" << std::endl;
  waitpid(kid, &res, 0);

  /*
  res will be set to 0 i.e., ls return code if ls ended correctly
  res will be set to some other number if ls ended in error and kid return 1
  #include <sys/types.h>
  #include <sys/wait.h>

  WIFEXITED(status)
  returns true if the child terminated normally, that is, by calling exit(3) or _exit(2), or by returning from main().
  WEXITSTATUS(status)
  returns the exit status of the child. This consists of the least significant 8 bits of the status argument that the child specified in a call to exit(3) or _exit(2) or as the argument for a return statement in main(). This macro should only be employed if WIFEXITED returned true.

  Clipped from: https://linux.die.net/man/2/waitpid
  */

  std::cout << "[exec] finished execution with return status[Non-readable]: " << res
            << std::endl;
  std::cout << "[exec] finished execution with return status: " << WEXITSTATUS(res)
            << std::endl;

  return 0;
}
