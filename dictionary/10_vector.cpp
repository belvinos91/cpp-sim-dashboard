// title: List of numbers -> std::vector
// category: Containers
// python:
//  x = [1.0, 2.0, 3.0]
//  x.append(4.0)
//  print(len(x), x[0], x[-1])
// cpp:
//  std::vector<double> x{1.0, 2.0, 3.0};
//  x.push_back(4.0);
//  std::cout << x.size() << x[0] << x.back();
// note: `std::vector` is C++'s list: `append` is `push_back`, `len` is `.size()`, `x[-1]` is `x.back()`. `x[i]` does not check bounds; `x.at(i)` does and reports an error.
// ---
#include <iostream>
#include <vector>

int main() {
    std::vector<double> x{1.0, 2.0, 3.0};
    x.push_back(4.0);
    std::cout << "size " << x.size() << ", first " << x[0] << ", last " << x.back() << "\n";
    std::vector<double> zeros(5, 0.0);                  // like [0.0] * 5
    std::cout << "zeros has " << zeros.size() << " items\n";
    return 0;
}
