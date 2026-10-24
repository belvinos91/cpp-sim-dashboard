// title: Print to the screen
// category: Basics
// python:
//  x = 3.5
//  print("x =", x)
// cpp:
//  double x = 3.5;
//  std::cout << "x = " << x << "\n";
// note: `std::cout` is the screen; `<<` sends each piece to it. Use `"\n"` for a new line: C++ does not add one for you. `#include <iostream>` is needed.
// ---
#include <iostream>

int main() {
    double x = 3.5;
    std::cout << "x = " << x << "\n";
    return 0;
}
