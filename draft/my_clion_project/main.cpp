#include <iostream>

using namespace std;

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

        try
        {
            throw string("some error 123456");
        }
        catch (string error_msg)
        {
            cerr << error_msg << endl;
        }

        cout << line << endl;
    }
    return 0;
}
