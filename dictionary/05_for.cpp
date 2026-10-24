// title: for loop over a range
// category: Control flow
// python:
//  for i in range(5):
//      print(i)
// cpp:
//  for (int i = 0; i < 5; ++i) {
//      std::cout << i << "\n";
//  }
// note: The C++ loop has three parts: start (`int i = 0`), condition to continue (`i < 5`), and step (`++i`, which adds 1). `range(5)` stops before 5, and so does `i < 5`.
// ---
#include <iostream>

int main() {
    for (int i = 0; i < 5; ++i) {
        std::cout << i << "\n";
    }
    return 0;
}
