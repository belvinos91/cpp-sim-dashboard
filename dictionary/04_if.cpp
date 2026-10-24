// title: if / elif / else
// category: Control flow
// python:
//  if R0 > 1:
//      print("epidemic")
//  elif R0 == 1:
//      print("threshold")
//  else:
//      print("dies out")
// cpp:
//  if (R0 > 1) {
//      std::cout << "epidemic\n";
//  } else if (R0 == 1) {
//      std::cout << "threshold\n";
//  } else {
//      std::cout << "dies out\n";
//  }
// note: Conditions go in round brackets, blocks in curly braces. `and`, `or`, `not` are written `&&`, `||`, `!`.
// ---
#include <iostream>

int main() {
    for (double R0 : {0.8, 1.0, 2.5}) {
        std::cout << "R0 = " << R0 << ": ";
        if (R0 > 1) {
            std::cout << "epidemic\n";
        } else if (R0 == 1) {
            std::cout << "threshold\n";
        } else {
            std::cout << "dies out\n";
        }
    }
    return 0;
}
