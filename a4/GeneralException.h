//
// Created by Yulin Wu on 2024-09-28.
//

#ifndef GENERALEXCEPTION_H
#define GENERALEXCEPTION_H

#include <exception>
#include <string>
#include <utility>

namespace a4 {
    class GeneralException : public std::exception {
    private:
        std::string message;

    public:
        explicit GeneralException(std::string message) : message(std::move(message)) {
        };

        const char *what() const noexcept override {
            return message.c_str();
        }
    };
} // a2

#endif //GENERALEXCEPTION_H
