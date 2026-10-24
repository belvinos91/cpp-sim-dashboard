// title: Maths functions
// category: Numerics
// python:
//  import math
//  math.exp(1), math.sqrt(2), 2 ** 10, abs(-3.5)
// cpp:
//  #include <cmath>
//  std::exp(1.0), std::sqrt(2.0), std::pow(2.0, 10), std::fabs(-3.5)
// note: `<cmath>` replaces `import math`. There is no `**` operator: use `std::pow`, or plain `x * x` for squares (faster).
// ---
#include <cmath>
#include <iostream>

int main() {
    std::cout << "exp(1)   = " << std::exp(1.0) << "\n";
    std::cout << "sqrt(2)  = " << std::sqrt(2.0) << "\n";
    std::cout << "2^10     = " << std::pow(2.0, 10) << "\n";
    std::cout << "|-3.5|   = " << std::fabs(-3.5) << "\n";
    std::cout << "pi       = " << std::acos(-1.0) << "\n";
    return 0;
}
