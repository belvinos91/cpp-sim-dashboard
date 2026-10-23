// Logistic growth dx/dt = r x (1 - x/K), solved with Euler's method
// and compared with the exact solution. Change h and watch the error.
#include <cmath>
#include <fstream>
#include <iostream>

int main() {
    double r = 0.8, K = 100, x0 = 2, h = 0.5, T = 15;
    double x = x0, maxErr = 0;
    std::ofstream out("data.csv");
    out << "t,Euler,exact\n";
    for (double t = 0; t <= T + 1e-12; t += h) {
        double exact = K / (1 + (K / x0 - 1) * std::exp(-r * t));
        out << t << "," << x << "," << exact << "\n";
        maxErr = std::fmax(maxErr, std::fabs(x - exact));
        x += h * r * x * (1 - x / K);              // Euler step
    }
    std::cout << "step h = " << h << ", largest error = " << maxErr << "\n";
    return 0;
}
