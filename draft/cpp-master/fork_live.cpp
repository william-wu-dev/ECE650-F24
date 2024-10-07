// C++ Program to demonstrate  
// a multiprocessing environment. 
#include <iostream> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>

using namespace std; 

int main() 
{ 
    std::cout << "Line before fork" << std::endl;
    pid_t c_pid = fork(); 

    if (c_pid == -1) { 
        perror("fork"); 
        // Clipped from: https://www.ibm.com/docs/en/i/7.3?topic=functions-perror-print-error-message
        // perror() — Print Error Message
        // Defined in header <cstdio>
        // The message associated with the value in errno is then printed followed by a new-line character.
        exit(EXIT_FAILURE); 
        return 1; 
    } 
    else if (c_pid > 0) { 
        wait(nullptr);
        /*
        wait() and waitpid()
        The wait() system call suspends execution of the calling process until one of its children terminates. The call wait(&status) is equivalent to:
        waitpid(-1, &status, 0);
        
        Clipped from: https://linux.die.net/man/2/waitpid
        */
        perror("fork"); 
        cout << "printed from parent process " << getpid() 
            << endl;

        cout << "parent pid: " << getppid() << endl; return 0;
    } 
    else { 
        cout << "printed from child process " << getpid() 
            << endl;
        cout << "parent pid: " << getppid() << endl;
        return 0;
    } 

}

