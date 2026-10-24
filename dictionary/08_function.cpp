// title: Define a function
// category: Functions
// python:
//  def f(x):
//      return x * x
//  
//  print(f(3.0))
// cpp:
//  double f(double x) {
//      return x * x;
//  }
//  
//  std::cout << f(3.0);
// note: Write the return type before the name and the type of each argument. The function must be defined (or declared) before `main` uses it.
// ---
#include <iostream>

double f(double x) {
    return x * x;
}

int main() {
    std::cout << "f(3) = " << f(3.0) << "\n";
    return 0;
}
