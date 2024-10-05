#include <unistd.h>
#include <iostream>
#include <sys/types.h>  // for waitpid
#include <sys/wait.h>  // for waitpid
int child() {
    char *args[3];
    args[0] = "ls";
    args[1] = "-l";
    args[2] = nullptr;
    /// call 'ls -l'
    execv("/bin/ls", args);
    std::cerr
        << "pid: " << getpid()
        << " Error: failed to execute ls" << std::endl;
    return 1;
}
int main(void) {
    std::cout << "my pid: " << getpid() << "\n"
              << "mu parent pid: " << getppid() << std::endl;

    // -- create a one-directional pipe
    // CtoP[0] is READ end of the pipe
    // CtoP[1] is WRITE end of the pipe
    int CtoP[2];
    pipe(CtoP);

    auto pid = fork();
    if (pid == 0) {
        dup2(CtoP[1], STDOUT_FILENO);
        close(CtoP[0]);
        close(CtoP[1]);

        std::cout << "I am in a child process" << std::endl;
        std::cout << "[C]: start to sleep for 10s" << std::endl;
        sleep(10);
        return child();
    } else if (pid > 0) {
        std::cout << "I am a parent process, and created child: "
                  << pid << std::endl;
    }
    else
    {
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }

    // only parent gets to here
    dup2(CtoP[0], STDIN_FILENO);
    close(CtoP[0]);
    close(CtoP[1]);

    // read input from the child until eof
    while (!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        std::cout << "[P]: " << line << std::endl;
    }

    int status;
    std::cout << "Parent waiting for child: " << pid << std::endl;
    waitpid(pid, &status, 0);
    std::cout << "Child terminated with status: " << status << std::endl;

    return 0;
}