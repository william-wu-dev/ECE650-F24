#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <vector>
#include "Graph.h"
#include "GeneralException.h"
#include <signal.h>

#define END_LINE_ENABLE true
#define IGNORE_COMMENT true
#define DEBUG false
#define WAIT_FOR_APPROX true


enum State {
    START,
    V_SPECIFIED,
    E_SPECIFIED
};

const char COMMA = ',';
const char LT = '<';
const char GT = '>';
const char LB = '{';
const char RB = '}';

const int SLEEP_TIME = 2;

/**
 * data structure for threads
 */
struct Data {
    Data(project::Graph *graph, std::vector<int> *result)
        : graph(graph),
          result(result) {
    }

    project::Graph *graph;
    std::vector<int> *result;

    Data() = default;
};

/**
 * thread that runs CNF-SAT-VC job
 * @param _data IO data for this thread job
 * @return N/A
 */
void *CNFSatVCRun(void *_data) {
    // convert data
    const auto data = static_cast<Data *>(_data);

    // compute CNF-SAT-VC
    *data->result = data->graph->CNFSatVC();

    return nullptr;
}

/**
 * thread that runs APPROX-VC-1 job
 * @param _data IO data for this thread job
 * @return N/A
 */
void *ApproxVC1Run(void *_data) {
    // convert data
    const auto data = static_cast<Data *>(_data);

    // compute ApproxVC1
    *data->result = data->graph->ApproxVC1();

    return nullptr;
}

/**
 * thread that runs APPROX-VC-2 job
 * @param _data IO data for this thread job
 * @return N/A
 */
void *ApproxVC2Run(void *_data) {
    // convert data
    const auto data = static_cast<Data *>(_data);

    // compute ApproxVC1
    *data->result = data->graph->ApproxVC2();

    return nullptr;
}

/**
 * This is the main thread, it deals with IO and creates three other threads for solving the VC problem
 * @param argc Not required
 * @param argv Not required
 * @return 0 for success, non-0 otherwise
 */
int main(int argc, char **argv) {
    // initialize Finite State Machine
    auto state = START;

    // allocate the graph on heap so that threads can share this graph
    std::unique_ptr<project::Graph> graph(new project::Graph(0));
    // allocate output vector so that the main thread can obtain results
    std::unique_ptr<std::vector<int> > CNFSatVCResult(new std::vector<int>());
    std::unique_ptr<std::vector<int> > ApproxVC1Result(new std::vector<int>());
    std::unique_ptr<std::vector<int> > ApproxVC2Result(new std::vector<int>());

    // read from stdin until EOF
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);

        // ignore empty line
        if (line.empty()) {
            // break;
            // @260: test case will end with an empty line, should terminate
            continue;
        }

#if IGNORE_COMMENT
        // ignore comment line
        if (line[0] == '#') {
            continue;
        }
#endif


        // create an input stream based on the line
        // we will use the input stream to parse the line
        std::istringstream input(line);

        // actions based on FSM
        try {
            switch (state) {
                case START: {
                    // V specification is expected
                    char command;
                    input >> command;
                    if (input.fail()) {
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        throw project::GeneralException("Unable to read command.");
                    }
                    if (command != 'V') {
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        std::string message = "expect \'V\' to start a graph specification, but receive: ";
                        message += command;
                        message += ".";
                        throw project::GeneralException(message);
                        continue;
                    }

                    // read vertex specification
                    int vertexCount;
                    input >> vertexCount;
                    // check vertex specification validity
                    if (input.fail()) {
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        std::string message = "unable to read vertex specification in command: ";
                        message += line;
                        message += ".";
                        throw project::GeneralException(message);
                    }
                    // NOTE: do not just use eof here, because previous read is just for one character, input only know that
                    // the reading was a success, not knowing that it is now at EOF. So, we force it to read something,
                    // and input will know that it hits an EOF or not. If hits an EOF, reading would be failed.
                    char dummy;
                    input >> dummy;
                    if (!input.eof()) {
                        // unexpected symbols following specification
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        std::string message = "encounter unexpected argument in command: ";
                        message += line;
                        message += ".";
                        throw project::GeneralException(message);
                    }
                    // if all checked, set graph
                    try {
                        graph->reset(vertexCount);
                    } catch (std::exception &e) {
                        state = START;
                        throw; // rethrow the exception.
                    }
                    // change state
                    state = V_SPECIFIED;
                    break;
                }
                case V_SPECIFIED: {
                    // E specification is expected
                    char command;
                    input >> command;
                    if (input.fail()) {
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        throw project::GeneralException("Unable to read command.");
                    }
                    if (command != 'E') {
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        std::string message = "expect \'E\' to specify edges, but receive: ";
                        message += command;
                        message += ".";
                        throw project::GeneralException(message);
                    }

                    // read edge specification
                    char left_brace;
                    input >> left_brace;
                    if (input.fail()) {
                        // read failed
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        std::string message = "unable to read edge specification in command: ";
                        message += line;
                        message += ".";
                        throw project::GeneralException(message);
                    }
                    if (left_brace != LB) {
                        // encounter unexpected starting symbol
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        std::string message = "expect \'{\' to start edges specification, but receive: ";
                        message += left_brace;
                        message += ".";
                        throw project::GeneralException(message);
                    }
                    // use flag to control the edge parsing process
                    bool flag;
                    // since graph without an edge is acceptable, we need to check whether there is edge in this command or not
                    if (input.peek() == 125) {
                        // we found a right brace after left brace; it means there's no edge.
                        char right_brace;
                        input >> right_brace;
                        flag = false;
                    } else {
                        flag = true;
                    }
                    // if facing comma after an edge, remain true; if facing right brace halt; otherwise, error
                    while (flag) {
                        // read lt
                        char less_than;
                        input >> less_than;
                        if (input.fail()) {
                            // reading failed
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "unable to read a \'<\' in command: ";
                            message += line;
                            message += ".";
                            throw project::GeneralException(message);
                        }
                        if (less_than != LT) {
                            // not lt symbol
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "expect \'<\' in the specification of an edge, but receive: ";
                            message += less_than;
                            message += ".";
                            throw project::GeneralException(message);
                        }

                        // read a number
                        int from;
                        input >> from;
                        if (input.fail()) {
                            // reading failed
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "unable to read the starting vertex of an edge in command: ";
                            message += line;
                            message += ".";
                            throw project::GeneralException(message);
                        }

                        // read a comma
                        char comma;
                        input >> comma;
                        if (input.fail()) {
                            // reading failed
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "unable to read a \',\' in command: ";
                            message += line;
                            message += ".";
                            throw project::GeneralException(message);
                        }
                        if (comma != COMMA) {
                            // not comma symbol
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "expect \',\' in the specification of an edge, but receive: ";
                            message += comma;
                            message += ".";
                            throw project::GeneralException(message);
                        }

                        // read another number
                        int to;
                        input >> to;
                        if (input.fail()) {
                            // reading failed
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "unable to read the ending vertex of an edge in command: ";
                            message += line;
                            message += ".";
                            throw project::GeneralException(message);
                        }

                        // read gt symbol
                        char greater_than;
                        input >> greater_than;
                        if (input.fail()) {
                            // reading failed
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "unable to read a \'>\' in command: ";
                            message += line;
                            message += ".";
                            throw project::GeneralException(message);
                        }
                        if (greater_than != GT) {
                            // not lt symbol
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "expect \'>\' in the specification of an edge, but receive: ";
                            message += greater_than;
                            message += ".";
                            throw project::GeneralException(message);
                        }

                        // complete reading an edge, add it
                        try {
                            graph->addEdge(from, to, 1);
                        } catch (std::exception &e) {
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            throw; // rethrow the exception to handle the print-out
                        }

                        // use the next symbol to determine whether loop should continue or halt
                        char indicator;
                        input >> indicator;
                        if (input.fail()) {
                            // reading failed
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = R"(unable to read a edge-separator ',' or end sign '}' in command: )";
                            message += line;
                            message += ".";
                            throw project::GeneralException(message);
                        }
                        if (indicator != COMMA && indicator != RB) {
                            // unknown symbol
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "encounter unexpected symbol between edges: ";
                            message += indicator;
                            message += ".";
                            throw project::GeneralException(message);
                        }

                        if (indicator == COMMA) {
                            flag = true;
                        } else if (indicator == RB) {
                            flag = false;
                        } else {
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            throw project::GeneralException(R"(unexpected error.)");
                        }
                    }

                    // check if there is unexpected argument following
                    // NOTE: do not just use eof here, because previous read is just for one character, input only know that
                    // the reading was a success, not knowing that it is now at EOF. So, we force it to read something,
                    // and input will know that it hits an EOF or not. If hits an EOF, reading would be failed.
                    char dummy;
                    input >> dummy;
                    if (!input.eof()) {
                        // unexpected symbols following specification
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        std::string message = "encounter unexpected argument in command: ";
                        message += line;
                        message += ".";
                        throw project::GeneralException(message);
                    }

                    // initialize output vector
                    CNFSatVCResult->clear();
                    ApproxVC1Result->clear();
                    ApproxVC2Result->clear();

                    // prepare data for each thread
                    Data CNFSatVCData((graph.get()), (CNFSatVCResult.get()));
                    Data ApproxVC1Data((graph.get()), (ApproxVC1Result.get()));
                    Data ApproxVC2Data((graph.get()), (ApproxVC2Result.get()));

                    // creat thread
                    pthread_t CNFSatVCThread;
                    pthread_create(&CNFSatVCThread, nullptr, &CNFSatVCRun, &CNFSatVCData);
                    pthread_t ApproxVC1Thread;
                    pthread_create(&ApproxVC1Thread, nullptr, &ApproxVC1Run, &ApproxVC1Data);
                    pthread_t ApproxVC2Thread;
                    pthread_create(&ApproxVC2Thread, nullptr, &ApproxVC2Run, &ApproxVC2Data);

                    // wait for a moment
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));

#if WAIT_FOR_APPROX
                    // makesure we will get the result from approx
                    pthread_join(ApproxVC1Thread, nullptr);
                    pthread_join(ApproxVC2Thread, nullptr);
#endif

                    // see if it's still there or kill everyone
                    if (!pthread_kill(CNFSatVCThread, 0)) {
                        // signal 0 means send no signal, just check
                        // returns 0 means the thread is still there, otherwise the thread is gone
                        // wait for CNF a little bit more
                        std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME));
                        // we will cancel the thread if it's still there
                        pthread_cancel(CNFSatVCThread);
                    }
#if !WAIT_FOR_APPROX
                    if (!pthread_kill(ApproxVC1Thread, 0)) {
                        // signal 0 means send no signal, just check
                        // returns 0 means the thread is still there, otherwise the thread is gone
                        // we will cancel the thread if it's still there
                        auto status = pthread_cancel(ApproxVC1Thread);
                        if (status != 0) {
                            // cancel that thread ended in error
                            perror("Error");
                        }
                    }
                    if (!pthread_kill(ApproxVC2Thread, 0)) {
                        // signal 0 means send no signal, just check
                        // returns 0 means the thread is still there, otherwise the thread is gone
                        // we will cancel the thread if it's still there
                        auto status = pthread_cancel(ApproxVC2Thread);
                        if (status != 0) {
                            // cancel that thread ended in error
                            perror("Error");
                        }
                    }
#endif

                    // collect produced result
                    // collect vertex cover result and output
                    std::string result_str;

                    // generate result string CNF-SAT-VC
                    result_str += "CNF-SAT-VC: ";
                    if (CNFSatVCResult->empty()) {
                        // killed by timeout
                        result_str += "timeout";
                    } else {
                        for (size_t i = 0; i < CNFSatVCResult->size(); i++) {
                            if (i) {
                                result_str += ",";
                            }
                            result_str += std::to_string(CNFSatVCResult->at(i));
                        }
                    }
                    result_str += "\n";

                    // generate result string APPROX-VC-1
                    result_str += "APPROX-VC-1: ";
                    if (ApproxVC1Result->empty()) {
                        // killed by timeout
                        result_str += "timeout";
                    } else {
                        for (size_t i = 0; i < ApproxVC1Result->size(); i++) {
                            if (i) {
                                result_str += ",";
                            }
                            result_str += std::to_string(ApproxVC1Result->at(i));
                        }
                    }
                    result_str += "\n";

                    // generate result string APPROX-VC-2
                    result_str += "APPROX-VC-2: ";
                    if (ApproxVC2Result->empty()) {
                        // killed by timeout
                        result_str += "timeout";
                    } else {
                        for (size_t i = 0; i < ApproxVC2Result->size(); i++) {
                            if (i) {
                                result_str += ",";
                            }
                            result_str += std::to_string(ApproxVC2Result->at(i));
                        }
                    }
#if END_LINE_ENABLE
                    result_str += "\n";
#endif
                    std::cout << result_str <<std::flush;

                    // TODO:collect analysis result

#if DEBUG
                    std::cerr << graph->toString() << std::endl << std::flush;
#endif


                    // set state to edge specified
                    state = E_SPECIFIED;
                    break;
                }
                case E_SPECIFIED: {
                    // REGULATION:
                    // in this state, only V command will and is bound to change the state to V specified or start (if V command is error),
                    // other command, no matter correct or error
                    // will not change the state.
                    char command;
                    input >> command;
                    if (input.fail()) {
                        // remain at this state because a graph is already specified.
                        state = E_SPECIFIED;
                        throw project::GeneralException("Unable to read command.");
                    }
                    if (command != 'V') {
                        // reset FSM to start over, because VE occurs together
                        state = E_SPECIFIED;
                        std::string message =
                                R"(expect 'V' to start a graph specification, but receive: )";
                        message += command;
                        message += ".";
                        throw project::GeneralException(message);
                    }

                    if (command == 'V') {
                        // if receive V command, reset graph and change state to V specified.
                        // read vertex specification
                        int vertexCount;
                        input >> vertexCount;
                        // check vertex specification validity
                        if (input.fail()) {
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "unable to read vertex specification in command: ";
                            message += line;
                            message += ".";
                            throw project::GeneralException(message);
                        }
                        // NOTE: do not just use eof here, because previous read is just for one character, input only know that
                        // the reading was a success, not knowing that it is now at EOF. So, we force it to read something,
                        // and input will know that it hits an EOF or not. If hits an EOF, reading would be failed.
                        char dummy;
                        input >> dummy;
                        if (!input.eof()) {
                            // unexpected symbols following specification
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "encounter unexpected argument in command: ";
                            message += line;
                            message += ".";
                            throw project::GeneralException(message);
                        }
                        // if all checked, set graph
                        try {
                            graph->reset(vertexCount);
                        } catch (std::exception &e) {
                            state = START;
                            throw; // rethrow the exception.
                        }
                        // change state
                        state = V_SPECIFIED;
                    } else {
                        state = E_SPECIFIED;
                        throw project::GeneralException(R"(unexpected error.)");
                    }
                    break;
                }
            }
        } catch (std::exception &e) {
#if END_LINE_ENABLE
            std::cerr << "Error: " << e.what() << std::endl << std::flush;
#else
            std::cerr << "Error: " << e.what() << std::flush;
#endif
        }
    }
    return 0;
}
