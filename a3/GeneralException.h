//
// Created by Yulin Wu on 2024-09-28.
//

#ifndef GENERALEXCEPTION_H_A3
#define GENERALEXCEPTION_H_A3

#include <exception>
#include <string>
#include <utility>

namespace a3{
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
} // a3

#endif //GENERALEXCEPTION_H_A3
