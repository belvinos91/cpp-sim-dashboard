// title: Variables and types
// category: Basics
// python:
//  n = 10          # int
//  x = 2.5         # float
//  name = "SIR"    # str
//  ok = True       # bool
// cpp:
//  int n = 10;
//  double x = 2.5;
//  std::string name = "SIR";
//  bool ok = true;
// note: In C++ every variable has a fixed type, written before its name. Python's `float` is C++'s `double`. Use `auto` to let the compiler infer the type: `auto y = 2.5;` is a `double`.
// ---
#include <iostream>
#include <string>

int main() {
    int n = 10;
    double x = 2.5;
    std::string name = "SIR";
    bool ok = true;
    auto y = x * n;                       // auto: compiler infers double
    std::cout << name << ": n=" << n << " x=" << x << " y=" << y
              << " ok=" << std::boolalpha << ok << "\n";
    return 0;
}
