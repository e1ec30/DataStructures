#include "List.h"

int main() {
  List<int> l;

  for(int i = 0; i < 10; i++) l.insertFront(i);
  l.reverse();

  l.print(std::cout);
  std::cout << std::endl;

  l.reverseNth(2);

  l.print(std::cout);
  std::cout << std::endl;
}
