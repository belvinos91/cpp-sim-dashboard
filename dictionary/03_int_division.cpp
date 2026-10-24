// title: Integer division trap
// category: Basics
// python:
//  print(1 / 2)    # 0.5
//  print(1 // 2)   # 0
// cpp:
//  std::cout << 1 / 2;      // 0  (both int!)
//  std::cout << 1.0 / 2;    // 0.5
// note: In C++, `int / int` is integer division, like Python's `//`. Write at least one number as a `double` (`1.0`) to get 0.5. A classic bug in rates such as `beta * S * I / N` when S, I, N are `int`.
// ---
#include <iostream>

int main() {
    int S = 990, I = 10, N = 1000;
    std::cout << "1 / 2   = " << 1 / 2 << "\n";
    std::cout << "1.0 / 2 = " << 1.0 / 2 << "\n";
    std::cout << "S * I / N          = " << S * I / N << "   (int: wrong)\n";
    std::cout << "double(S) * I / N  = " << double(S) * I / N << "   (correct)\n";
    return 0;
}
