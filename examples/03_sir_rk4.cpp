// SIR model with Runge-Kutta 4 (fractions of the population). R0 = beta/gamma.
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>

using State = std::array<double, 3>;               // S, I, R
struct Params { double beta, gamma; };

State rhs(const State& x, const Params& p) {
    double inf = p.beta * x[0] * x[1], rec = p.gamma * x[1];
    return { -inf, inf - rec, rec };
}
State step(const State& x, const State& k, double h) {
    return { x[0] + h * k[0], x[1] + h * k[1], x[2] + h * k[2] };
}

int main() {
    Params p{0.3, 0.1};
    State x{0.99, 0.01, 0.0};
    double h = 0.1, T = 160, worst = 0;
    std::ofstream out("data.csv");
    out << "day,S,I,R\n";
    for (double t = 0; t <= T + 1e-12; t += h) {
        out << t << "," << x[0] << "," << x[1] << "," << x[2] << "\n";
        State k1 = rhs(x, p), k2 = rhs(step(x, k1, h / 2), p),
              k3 = rhs(step(x, k2, h / 2), p), k4 = rhs(step(x, k3, h), p);
        for (int i = 0; i < 3; ++i) x[i] += h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
        worst = std::fmax(worst, std::fabs(x[0] + x[1] + x[2] - 1));
    }
    std::cout << "R0 = " << p.beta / p.gamma << ", final size R(T) = " << x[2]
              << ", largest |S+I+R-1| = " << worst << "\n";
    return 0;
}
