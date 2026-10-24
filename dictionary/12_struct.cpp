// title: Group parameters -> struct
// category: Containers
// python:
//  from dataclasses import dataclass
//  
//  @dataclass
//  class Params:
//      beta: float
//      gamma: float
// cpp:
//  struct Params {
//      double beta;
//      double gamma;
//  };
//  Params p{0.3, 0.1};
// note: A `struct` bundles named values: `p.beta` instead of a long list of arguments. One `struct` is clearer and safer than a function with many arguments in a fixed order.
// ---
#include <iostream>

struct Params {
    double beta;
    double gamma;
};

double R0(const Params& p) { return p.beta / p.gamma; }

int main() {
    Params p{0.3, 0.1};
    std::cout << "beta=" << p.beta << " gamma=" << p.gamma << " R0=" << R0(p) << "\n";
    return 0;
}
