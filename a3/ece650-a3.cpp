#include <iostream>
#include <csignal>
#include <unistd.h>
#include <vector>
#include <string.h>

#define DEBUG true

/**
 * This is the driver program. Its job is to:
 * 1. fork and create processes,
 *    connect pipes,
 *    run execution (pass arguments to rgen),
 *    print error if anything happens
 * 2. relay input to a2, so this process will eventually become as a relay station.
 * 3. if it sees an eof in the input, kills all other processes with sigterm.
 *    So, I need a bookkeeping of all the processes.
 *
 *    FIXME: it might cause buggy error report if you signal termination
 *
 * Regulation: We do not check error in: pipe, dup2, close
 * @param argc argument count for rgen
 * @param argv argument vector of rgen
 * @return
 */
int main (int argc, char **argv) {
    // bookkeeping all the processes id
    std::vector<pid_t> pids;

    // define pipes
    int rgen_to_a1[2];
    pipe(rgen_to_a1);
    int a1_and_driver_to_a2[2];
    pipe(a1_and_driver_to_a2);

    // creating processes

    // create a1
    const pid_t a1_pid = fork();
    if (a1_pid == 0) {
        // connect pipe, because if you exec, everything is gone
        dup2(rgen_to_a1[0], STDIN_FILENO);
        close(rgen_to_a1[1]);
        close(rgen_to_a1[0]);

        dup2(a1_and_driver_to_a2[1], STDOUT_FILENO);
        close(a1_and_driver_to_a2[0]);
        close(a1_and_driver_to_a2[1]);

        // call a1
        char path_to_a1[100];
        strcpy(path_to_a1, "./a1ece650.py");
        char *argv_for_a1[2];
        argv_for_a1[0] = path_to_a1;
        argv_for_a1[1] = nullptr;

#if DEBUG
        std::cerr << "start to execv a1" << std::endl;
#endif

        execv(path_to_a1, argv_for_a1);

        // FIXME: this might lead to problems because we don't know sigterm will set error or not
        perror("Error: executing a1");

        return EXIT_FAILURE;
    } else if (a1_pid < 0) {
        // creating a1 process ended up in error
        perror("Error: creating a1 process");
        return EXIT_FAILURE;
    } else {
        // bookkeeping in parent
        pids.push_back(a1_pid);
    }


    // create a2
    const pid_t a2_pid = fork();
    if (a2_pid == 0) {
        // connect pipe, because if you exec, everything is gone
        dup2(a1_and_driver_to_a2[0], STDIN_FILENO);
        close(a1_and_driver_to_a2[1]);
        close(a1_and_driver_to_a2[0]);

        // call a1
        char path_to_a2[100];
        strcpy(path_to_a2, "./ece650-a2");
        char *argv_for_a2[2];
        argv_for_a2[0] = path_to_a2;
        argv_for_a2[1] = nullptr;

#if DEBUG
        std::cerr << "start to execv a2" << std::endl;
#endif

        execv(path_to_a2, argv_for_a2);

        // FIXME: this might lead to problems because we don't know sigterm will set error or not
        perror("Error: executing a2");
        
        return EXIT_FAILURE;
    } else if (a2_pid < 0) {
        // creating a2 process ended up in error
        perror("Error: creating a2 process");
        return EXIT_FAILURE;
    } else {
        // bookkeeping in parent
        pids.push_back(a2_pid);
    }

    // create rgen
    const pid_t rgen_pid = fork();
    if (rgen_pid == 0) {
        // connect pipe, because if you exec, everything is gone
        dup2(rgen_to_a1[1], STDOUT_FILENO);
        close(rgen_to_a1[0]);
        close(rgen_to_a1[1]);

        // call rgen
        char path_to_rgen[100];
        strcpy(path_to_rgen, "./rgen");
        char *argv_for_rgen[argc + 1];
        argv_for_rgen[0] = path_to_rgen;
        for (int i = 1; i < argc; i++) {
            argv_for_rgen[i] = argv[i];
        }
        argv_for_rgen[argc] = nullptr;

#if DEBUG
        std::cerr << "start to execv rgen" << std::endl;
#endif
        execv(path_to_rgen, argv_for_rgen);

        // FIXME: this might lead to problems because we don't know sigterm will set error or not
        perror("Error: executing rgen");

        return EXIT_FAILURE;
    } else if (rgen_pid < 0) {
        // creating rgen process ended up in error
        perror("Error: creating rgen process");
        return EXIT_FAILURE;
    } else {
        // bookkeeping in parent
        pids.push_back(rgen_pid);
    }

    // this parent process becomes a relay station
    // connect pipe for relay station
    dup2(a1_and_driver_to_a2[1], STDOUT_FILENO);
    close(a1_and_driver_to_a2[0]);
    close(a1_and_driver_to_a2[1]);

#if DEBUG
        std::cerr << "start to relay all input" << std::endl;
#endif

    // begin the relay, until eof
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        if (!line.empty()) {
            if (line[0] != '#') {
                // relay if not comment
                std::cout << line << std::endl;
            }
        }
    }

    // sees eof, sigterm to every other processes
    for (auto pid : pids) {
        kill(pid, SIGTERM);
        // kill(pid, SIGKILL);
    }
    
    return EXIT_SUCCESS;
}
