// title: One Euler step of an ODE
// category: Numerics
// python:
//  x = x + h * f(t, x)
//  t = t + h
// cpp:
//  x += h * f(t, x);
//  t += h;
// note: The update rule is identical in both languages. In C++ the loop runs compiled, typically 50 to 100 times faster than the same loop in plain Python.
// ---
#include <cmath>
#include <iostream>

double f(double /*t*/, double x) { return -0.5 * x; }   // dx/dt = -x/2

int main() {
    double t = 0, x = 1, h = 0.01;
    while (t < 2.0 - 1e-12) {
        x += h * f(t, x);
        t += h;
    }
    std::cout << "Euler x(2) = " << x << ", exact = " << std::exp(-1.0) << "\n";
    return 0;
}
