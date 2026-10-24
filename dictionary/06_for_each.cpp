// title: Loop over the items of a list
// category: Control flow
// python:
//  for x in values:
//      total += x
// cpp:
//  for (double x : values) {
//      total += x;
//  }
// note: The range-based `for` reads each item in turn, like Python's `for x in values`. Write `const double& x` for large items to avoid copying them.
// ---
#include <iostream>
#include <vector>

int main() {
    std::vector<double> values{1.5, 2.0, 3.5};
    double total = 0;
    for (double x : values) {
        total += x;
    }
    std::cout << "total = " << total << "\n";
    return 0;
}
