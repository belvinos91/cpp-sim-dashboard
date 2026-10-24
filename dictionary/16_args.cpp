// title: Read command-line arguments
// category: Files
// python:
//  import sys
//  n = int(sys.argv[1])
// cpp:
//  int main(int argc, char* argv[]) {
//      int n = std::stoi(argv[1]);
//  }
// note: `argv` holds the words typed after the program name; `argc` counts them, including the program name. `std::stoi` is `int(...)`, `std::stod` is `float(...)`. Type a number in the Arguments box to try it.
// ---
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    int n = (argc > 1) ? std::stoi(argv[1]) : 5;    // default 5 if no argument
    long long sum = 0;
    for (int i = 1; i <= n; ++i) sum += i;
    std::cout << "1 + ... + " << n << " = " << sum << "\n";
    return 0;
}
