#ifndef GENERAL_EXCEPTION_HPP
#define GENERAL_EXCEPTION_HPP

#include <exception>
#include <string>

class GeneralException : public std::exception
{
private:
     std::string errorMessage;

public:
     GeneralException(std::string _errorMessage) : errorMessage(_errorMessage) {};
     virtual const char *what() const throw()
     {
          return errorMessage.c_str();
     }
};

#endif // GENERAL_EXCEPTION_HPP
