#include <iostream>
#include "general_exception.hpp"
#include <exception>

using namespace std;

void foo() noexcept(false)
{
    throw GeneralException("some error in foo");
}

int main()
{
    while (!cin.eof())
    {
        string line;
        getline(cin, line);

        if (line.empty())
        {
            continue;
        }

        // try
        // {
        //     throw "some error 123456";
        // }
        // catch (const char *error_msg)
        // {
        //     cerr << error_msg << endl;
        // }

        // try
        // {
        //     throw string("some error 123456");
        // }
        // catch (string error_msg)
        // {
        //     cerr << error_msg << endl;
        // }

        // try
        // {
        //     throw GeneralException("some error 321654");
        // }
        // catch (exception &e)
        // {
        //     cerr << e.what() << endl;
        // }

        try
        {
            foo();
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
        }

        cout << line << endl;
    }
    return 0;
}
