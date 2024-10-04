// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Graph.h"
#include "GeneralException.h"


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

int main(int argc, char **argv) {
    // initialize Finite State Machine
    auto state = START;
    auto graph = a2::Graph(0); // initialize an empty graph
    // read from stdin until EOF
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);

        // ignore empty line
        if (line.empty()) {
            continue;
        }

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
                        throw a2::GeneralException("Unable to read command.");
                    }
                    if (command != 'V') {
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        // std::string message = "expect \'V\' to start a graph specification, but receive: ";
                        // message += command;
                        // message += ".";
                        // throw a2::GeneralException(message);
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
                        throw a2::GeneralException(message);
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
                        throw a2::GeneralException(message);
                    }
                    // if all checked, set graph
                    try {
                        graph.reset(vertexCount);
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
                        throw a2::GeneralException("Unable to read command.");
                    }
                    if (command != 'E') {
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        std::string message = "expect \'E\' to specify edges, but receive: ";
                        message += command;
                        message += ".";
                        throw a2::GeneralException(message);
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
                        throw a2::GeneralException(message);
                    }
                    if (left_brace != LB) {
                        // encounter unexpected starting symbol
                        // reset FSM to start over, because VE occurs together
                        state = START;
                        std::string message = "expect \'{\' to start edges specification, but receive: ";
                        message += left_brace;
                        message += ".";
                        throw a2::GeneralException(message);
                    }
                    // use flag to control the edge parsing process
                    bool flag;
                    // since graph without an edge is acceptable, we need to check whether there is edge in this command or not
                    if (input.peek() == 125) {  // we found a right brace after left brace; it means there's no edge.
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
                            throw a2::GeneralException(message);
                        }
                        if (less_than != LT) {
                            // not lt symbol
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "expect \'<\' in the specification of an edge, but receive: ";
                            message += less_than;
                            message += ".";
                            throw a2::GeneralException(message);
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
                            throw a2::GeneralException(message);
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
                            throw a2::GeneralException(message);
                        }
                        if (comma != COMMA) {
                            // not comma symbol
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "expect \',\' in the specification of an edge, but receive: ";
                            message += comma;
                            message += ".";
                            throw a2::GeneralException(message);
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
                            throw a2::GeneralException(message);
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
                            throw a2::GeneralException(message);
                        }
                        if (greater_than != GT) {
                            // not lt symbol
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "expect \'>\' in the specification of an edge, but receive: ";
                            message += greater_than;
                            message += ".";
                            throw a2::GeneralException(message);
                        }

                        // complete reading an edge, add it
                        try {
                            graph.addEdge(from, to, 1);
                        } catch (std::exception &e) {
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            throw; // rethrow the exception to handle the print out
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
                            throw a2::GeneralException(message);
                        }
                        if (indicator != COMMA && indicator != RB) {
                            // unknown symbol
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            std::string message = "encounter unexpected symbol between edges: ";
                            message += indicator;
                            message += ".";
                            throw a2::GeneralException(message);
                        }

                        if (indicator == COMMA) {
                            flag = true;
                        } else if (indicator == RB) {
                            flag = false;
                        } else {
                            // reset FSM to start over, because VE occurs together
                            state = START;
                            throw a2::GeneralException(R"(unexpected error.)");
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
                        throw a2::GeneralException(message);
                    }

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
                        throw a2::GeneralException("Unable to read command.");
                    }
                    if (command != 'V' && command != 's') {
                        // reset FSM to start over, because VE occurs together
                        state = E_SPECIFIED;
                        std::string message =
                                R"(expect 'V' to start a graph specification or 's' to generate shortest path, but receive: )";
                        message += command;
                        message += ".";
                        throw a2::GeneralException(message);
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
                            throw a2::GeneralException(message);
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
                            throw a2::GeneralException(message);
                        }
                        // if all checked, set graph
                        try {
                            graph.reset(vertexCount);
                        } catch (std::exception &e) {
                            state = START;
                            throw; // rethrow the exception.
                        }
                        // change state
                        state = V_SPECIFIED;
                    } else if (command == 's') {
                        // get from and to, generate the shortest path

                        // get from
                        int from;
                        input >> from;
                        if (input.fail()) {
                            // reading failed
                            state = E_SPECIFIED;
                            std::string message = "unable to read the starting vertex of an edge in command: ";
                            message += line;
                            message += ".";
                            throw a2::GeneralException(message);
                        }

                        // get to
                        int to;
                        input >> to;
                        if (input.fail()) {
                            // reading failed
                            state = E_SPECIFIED;
                            std::string message = "unable to read the ending vertex of an edge in command: ";
                            message += line;
                            message += ".";
                            throw a2::GeneralException(message);
                        }

                        // check unexpected following symbol
                        // NOTE: do not just use eof here, because previous read is just for one character, input only know that
                        // the reading was a success, not knowing that it is now at EOF. So, we force it to read something,
                        // and input will know that it hits an EOF or not. If hits an EOF, reading would be failed.
                        char dummy;
                        input >> dummy;
                        if (!input.eof()) {
                            // unexpected symbols following specification
                            state = E_SPECIFIED;
                            std::string message = "encounter unexpected argument in command: ";
                            message += line;
                            message += ".";
                            throw a2::GeneralException(message);
                        }

                        // generate the shortest graph
                        try {
                            std::cout << graph.compute(from, to) << std::endl;
                        } catch (std::exception &e) {
                            state = E_SPECIFIED; // exception in s will make the state remain at e specified.
                            throw; // rethrow the exception
                        }

                        // change state
                        state = E_SPECIFIED;
                    } else {
                        state = E_SPECIFIED;
                        throw a2::GeneralException(R"(unexpected error.)");
                    }
                    break;
                }
            }
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    return 0;
}
