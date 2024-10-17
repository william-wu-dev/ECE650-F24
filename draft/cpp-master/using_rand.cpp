// an example of reading random numbers from /dev/urandom
// https://stackoverflow.com/questions/35726331/c-extracting-random-numbers-from-dev-urandom
#include <fstream>
#include <iostream>
#include <assert.h>

#define OLD_ENABLE false
#define ERROR_DEMONSTRATION false
#define TEST_ENABLE false

const int M = 1e5 + 10;
int book[M];

int main(void) {

  // open /dev/urandom to read
  std::ifstream urandom("/dev/urandom");

  // check that it did not fail
  if (urandom.fail()) {
    std::cerr << "Error: cannot open /dev/urandom\n";
    return 1;
  }

#if OLD_ENABLE
  // read a random 8-bit value.
  // Have to use read() method for low-level reading
  char ch = 'a';
  urandom.read(&ch, 1);
  /*
  std::istream::read
  istream& read (char* s, streamsize n);
  */
  // cast to integer to see the numeric value of the character
  std::cout << "Random character: " << (unsigned int)ch << "\n";

  // read another 8-bit value
  urandom.read(&ch, 1);
  std::cout << "Random character: " << (unsigned int)ch << "\n";

  // read a random unsigned int
  unsigned int num = 42;
  urandom.read((char *)&num, sizeof(int));
  std::cout << "Random character: " << num << "\n";
#endif

#if ERROR_DEMONSTRATION
  // read a random integer
  // WARNING: DO NOT READ INT, CAUSING ENCODING ERROR
  int u = 0;
  urandom.read((char *)&u, sizeof(int));
  std::cout << "Random integer: " << u << std::endl;
#endif

  // generate a random integer in a range [a, b]
  const int a = -20, b = 20;
  const int N = 100;
  for (int i = 0; i < N; i++) {
    unsigned int x = 0;
    urandom.read((char *)&x, sizeof(int));
    // printf("random integer: %u in range [%d, %d]: %d\n", x, a, b, ((x % (b - a + 1)) + a));  // using inferred calculation is also ok.
    int mid = x % (unsigned int)(b - a + 1);  // calulated in unsigned then transfered to int. after the modulo, it is gauranteed that the result is in the int range. Unless, b - a + 1 exceed in range. 2147483647
    printf("mid: %u\n", mid);
    assert(mid <= (b - a) && mid >= 0);
    book[mid] += 1;
    int res = mid + a;
    printf("random integer: %u in range [%d, %d]: %d\n", x, a, b, res);
    assert(res <= b && res >= a);
  }

  for(int i = 0; i < (b - a + 1); i++) {
    printf("book[%d] = %d\n", i, book[i]);
  }

#if TEST_ENABLE
  std::cout << sizeof(int) << std::endl;
#endif

#if ERROR_DEMONSTRATION
  int test = 21474836470;  // int range is -2147483648 to 2147483647, this assignment will leads to nonsense value.
  std::cout << test << std::endl;
#endif
  // close random stream
  urandom.close();
  return 0;
}
