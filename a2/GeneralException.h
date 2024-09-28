//
// Created by Yulin Wu on 2024-09-28.
//

#ifndef GENERALEXCEPTION_H
#define GENERALEXCEPTION_H

#include <exception>
#include <string>

namespace a2 {
    class GeneralException : public std::exception {
    private:
        std::string message;

    public:
        GeneralException(std::string message) : message(message) {
        };

        virtual const char *what() const throw() {
            return message.c_str();
        }
    };
} // a2

#endif //GENERALEXCEPTION_H
