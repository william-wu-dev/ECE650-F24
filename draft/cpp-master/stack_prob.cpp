/** 
 * Complete an implementation of a stack
 */

// -- print using std::cout and std::cerr
#include <iostream>

// -- forward declaration of a stack
class Stack;

/// An entry of the stack
class StackEntry {
  /// Data stored in this stack entry
  int data;
  /// Pointer to the next stack entry
  StackEntry *next;

public:
  /// Constructor
  /// \param \p v a value to be addded to the stack
  /// \param \p en current top of the stack
  StackEntry(int v, StackEntry *n);

};

/// A stack
class Stack {
private:
  /// Number of elements in the stack
  int size;
  /// Pointer to the top of the stack
  StackEntry *top;

public:
  /// Constructor
  Stack();
  /// Destructor
  virtual ~Stack();

  /// Returns true if the stack is empty
  bool empty();
  /// Push element \p v to the top of the stack
  void push(int v);
  /// Return top of the stack and remove from the stack
  int pop();
};

/// Print stack \p s onto the output stream \p out
void print_stack(std::ostream &out, Stack *s);

/// Embedded unit test. Remove to use the Stack as a library
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("stack test 1") {
  Stack st;
  st.push(5);
  st.push(6);
  print_stack(std::cout, &st);
  CHECK(st.pop() == 6);
  CHECK(st.pop() == 5);
  CHECK(st.empty() == true);
}