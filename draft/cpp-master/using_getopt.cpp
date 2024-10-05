// adapted from
// https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
#include <iostream>
#include <unistd.h>

#define OLD_APPROACH false

/// main() must be declared with arguments
/// otherwise command line arguments are ignored
int main(int argc, char **argv) {
  bool aflag = 0;
  bool bflag = 0;
  std::string cvalue;
  int cint_value;
  std::string dvalue;
  int dint_value;
  int index;
  int c;

  opterr = 0;  
  // If getopt() does not recognize an option character, it prints an error message to stderr, stores the character in optopt, and returns '?'. The calling program may prevent the error message by setting opterr to 0.

  // Generally, the getopt() function is called from inside of a loop’s conditional statement. The loop terminates when the getopt() function returns -1. A switch statement is then executed with the value returned by getopt() function. 
  // expected options are '-a', '-b', and '-c value'
  while ((c = getopt(argc, argv, ":abc:d:")) != -1)  // ‘-1’ if there are no more options to process.
    switch (c) {
    case 'a':
      aflag = true;
      break;
    case 'b':
      bflag = true;
      break;
    case 'c':
      cvalue = optarg;  // If the option takes a value, that value is pointer to the external variable optarg
      cint_value = atoi(cvalue.c_str());
      /*The C stdlib library atoi() function is used to convert a numeric string into an integer value.*/
      break;
    case 'd':
      dvalue = optarg;  // If the option takes a value, that value is pointer to the external variable optarg
      dint_value = atoi(dvalue.c_str());
      /*The C stdlib library atoi() function is used to convert a numeric string into an integer value.*/
      break;
    case ':':
    /*
    this case will handle no argument error as if (optopt == 'c') would do
    */
      std::cerr << "Error: option -" << (char)optopt << " requires an argument. By colon"
                  << std::endl;  // you need to chage optopt to char 
      break;
    case '?':  // return ‘?’ when there is an unrecognized option and it stores into external variable optopt.
      #if OLD_APPROACH
      if (optopt == 'c')  // If an option requires a value (such as -f in our example) and no value is given, getopt normally returns ?
        std::cerr << "Error: option -" << (char)optopt << " requires an argument."
                  << std::endl;  // you need to chage optopt to char 
      else
      #endif
        std::cerr << "Error: unknown option: " << (char)optopt << std::endl;
      return 1;
    default:
      return 0;
    }

  std::cout << "aflag=" << aflag << " "
            << "bflag=" << bflag << " "
            << "cvalue=" << cvalue << " "
            << "cint_value=" << cint_value << " "
            << "dvalue=" << dvalue << " "
            << "dint_value=" << dint_value << std::endl;

  // optind is for the extra arguments 
  // which are not parsed 
  if (optind < argc) {
    std::cout << "Found positional arguments\n";
    for (index = optind; index < argc; index++)
      std::cout << "Non-option argument: " << argv[index] << "\n";
  }

  return 0;
}
