// title: while loop
// category: Control flow
// python:
//  t = 0.0
//  while t < 1.0:
//      t += 0.25
// cpp:
//  double t = 0.0;
//  while (t < 1.0) {
//      t += 0.25;
//  }
// note: Same idea as Python. Gillespie simulations use `while (I > 0)`: run events until the infection is gone.
// ---
#include <iostream>

int main() {
    double t = 0.0;
    int steps = 0;
    while (t < 1.0) {
        t += 0.25;
        ++steps;
    }
    std::cout << "t = " << t << " after " << steps << " steps\n";
    return 0;
}
