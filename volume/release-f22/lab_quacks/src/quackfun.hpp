/**
 * @file quackfun.cpp
 * This is where you will implement the required functions for the
 * stacks and queues portion of the lab.
 */

#include "quackfun.h"
#include <algorithm>
#include <cstdio>
#include <utility>
namespace QuackFun {

/**
 * Sums items in a stack.
 *
 * **Hint**: think recursively!
 *
 * @note You may modify the stack as long as you restore it to its original
 * values.
 *
 * @note You may use only two local variables of type T in your function.
 * Note that this function is templatized on the stack's type, so stacks of
 * objects overloading the + operator can be summed.
 *
 * @note We are using the Standard Template Library (STL) stack in this
 * problem. Its pop function works a bit differently from the stack we
 * built. Try searching for "stl stack" to learn how to use it.
 *
 * @param s A stack holding values to sum.
 * @return  The sum of all the elements in the stack, leaving the original
 *          stack in the same state (unchanged).
 */
template <typename T> T sum(stack<T> &s) {

  // Your code here

  if (s.size() == 1)
    return s.top();
  else {
    T top = s.top();
    s.pop();
    T result = sum(s);
    result += top;
    s.push(top);
    return result;
  }
  // stub return value (0 for primitive types). Change this!
  // Note: T() is the default value for objects, and 0 for
  // primitive types
}

/**
 * Checks whether the given string (stored in a queue) has balanced brackets.
 * A string will consist of square bracket characters, [, ], and other
 * characters. This function will return true if and only if the square bracket
 * characters in the given string are balanced. For this to be true, all
 * brackets must be matched up correctly, with no extra, hanging, or unmatched
 * brackets. For example, the string "[hello][]" is balanced, "[[][[]a]]" is
 * balanced, "[]]" is unbalanced, "][" is unbalanced, and "))))[cs225]" is
 * balanced.
 *
 * For this function, you may only create a single local variable of type
 * `stack<char>`! No other stack or queue local objects may be declared. Note
 * that you may still declare and use other local variables of primitive types.
 *
 * @param input The queue representation of a string to check for balanced
 * brackets in
 * @return      Whether the input string had balanced brackets
 */
bool isBalanced(queue<char> input) {

  // @TODO: Make less optimistic
  stack<char> s;
  while (!input.empty()) {
    char next = input.front();
    input.pop();
    if (next == '[')
      s.push(next);
    if (next == ']') {
      if (s.empty())
        return false;
      else
        s.pop();
    }
  }
  return s.empty();
}

/**
 * Reverses even sized blocks of items in the queue. Blocks start at size
 * one and increase for each subsequent block.
 *
 * **Hint**: You'll want to make a local stack variable.
 *
 * @note Any "leftover" numbers should be handled as if their block was
 * complete.
 *
 * @note We are using the Standard Template Library (STL) queue in this
 * problem. Its pop function works a bit differently from the stack we
 * built. Try searching for "stl stack" to learn how to use it.
 *
 * @param q A queue of items to be scrambled
 */
template <typename T> void scramble(queue<T> &q) {
  static int done = 0;
  static int n = 1;
  stack<T> s{};
  queue<T> q2{};

  // printf("size: %zu\n", q.size());
  // while (!q.empty()) {
  //   printf("%d, ", q.front());
  //   s.push(q.front());
  //   q2.push(q.front());
  //   q.pop();
  // }
  // puts("");
  //
  // printf("stack: ");
  //

  printf("done: %d, q.size(): %zu, n: %d\n", done, q.size(), n);
  if (done >= (int)q.size()) {
      done = 0;
      n = 1;
      return;
  }
  else {
    // int x = done;
    // e1ec30: save the work that's already been done.
    // while (x-- > 0 && !q.empty()) {
    //   printf("popping: %d\n", q.front());
    //   q2.push(q.front());
    //   q.pop();
    // }

    // e1ec30: check if we need to swap
    bool even = (n%2 == 0);
    if (done + n > (int)q.size()) n = q.size() - done;
    int x = n;
    if (even) {
      while (x-- > 0 && !q.empty()) {
        printf("popping: %d\n", q.front());
        s.push(q.front());
        q.pop();
      }

      while (!s.empty()) {
        printf("pushing: %d\n", s.top());
        q.push(s.top());
        s.pop();
      }
    }
    else {
        x = n;
        while (x-- > 0 && !q.empty()) {
            printf("popping: %d\n", q.front());
            printf("pushing: %d\n", q.front());
            q.push(q.front());
            q.pop();
        }
    }
    done += n;
    n += 1;
    scramble(q);
  }

  // Your code here
}
} // namespace QuackFun
