// Exact stochastic SIR (Gillespie algorithm), several runs from one infected person.
// About 1/R0 of runs die out early by chance: the ODE can never show this.
#include <fstream>
#include <iostream>
#include <random>

int main() {
    double beta = 0.3, gamma = 0.1;                // R0 = 3
    int N = 1000, runs = 8, dieOut = 0;
    std::mt19937 gen(7);
    std::uniform_real_distribution<double> U(0.0, 1.0);
    std::ofstream out("data.csv");
    out << "run,day,infected\n";

    for (int r = 1; r <= runs; ++r) {
        int S = N - 1, I = 1, R = 0;
        double t = 0;
        out << r << "," << t << "," << I << "\n";
        while (I > 0) {
            double aInf = beta * S * I / N, aRec = gamma * I, a0 = aInf + aRec;
            t += std::exponential_distribution<double>(a0)(gen);   // waiting time
            if (U(gen) * a0 < aInf) { --S; ++I; } else { --I; ++R; }
            out << r << "," << t << "," << I << "\n";
        }
        if (R < 50) ++dieOut;
        std::cout << "run " << r << ": total infected = " << R << "\n";
    }
    std::cout << dieOut << " of " << runs << " runs died out early (theory: about 1/R0 = 1/3)\n";
    return 0;
}
