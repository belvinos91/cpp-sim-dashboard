// Jump-driven prevalence, exact simulation (Lamperti-Mobius conjugacy).
// Two classes U (reference) and V (focal) with correlated multiplicative noise and
// common Poisson jumps. The logit Y = log(V/U) of prevalence P = V/(U+V) is
//   Y_t = Y_0 + alpha t + sigma_eff W_t + sum_{k <= N_t} Psi_k,   Psi_k ~ N(m, s^2),
// so it can be sampled exactly (no time step, no clipping of P to (0,1)).
// Plots 10 exact prevalence paths over 5 years; prints Monte Carlo vs exact values at T = 1.
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

// standard normal survival function
double normalSF(double z) { return 0.5 * std::erfc(z / std::sqrt(2.0)); }
double expit(double y) { return 1.0 / (1.0 + std::exp(-y)); }

int main() {
    // baseline parameters (time in years)
    double rU = 0.08, rV = 0.12, sU = 0.12, sV = 0.15, rho = 0.30, lambda = 2.0, P0 = 0.20;
    double mJU = -0.08, mJV = 0.04, sJ = 0.08, rhoJ = 0.25;    // Gaussian log-multipliers

    double sigEff = std::sqrt(sU * sU + sV * sV - 2 * rho * sU * sV);
    double alpha = rV - rU - 0.5 * (sV * sV - sU * sU);
    double m = mJV - mJU;                                        // mean logit jump
    double s = std::sqrt(2 * sJ * sJ * (1 - rhoJ));              // sd of logit jump
    double Y0 = std::log(P0 / (1 - P0));
    double Gamma = alpha + lambda * m;                            // long-run selection exponent

    std::mt19937_64 gen(20261028);                               // fixed seed: reproducible
    std::normal_distribution<double> N01(0.0, 1.0);
    std::uniform_real_distribution<double> U01(0.0, 1.0);

    // ---- exact terminal law at T = 1: Monte Carlo vs exact formulas ----
    double T = 1.0, Pc = 0.30;
    int draws = 50000;
    std::poisson_distribution<int> K1(lambda * T);
    double sY = 0, sY2 = 0, sP = 0, hits = 0;
    for (int i = 0; i < draws; ++i) {
        int k = K1(gen);
        double y = Y0 + alpha * T + sigEff * std::sqrt(T) * N01(gen) + k * m + s * std::sqrt((double)k) * N01(gen);
        sY += y; sY2 += y * y;
        double p = expit(y);
        sP += p;
        if (p >= Pc) hits += 1;
    }
    double meanY = sY / draws, varY = sY2 / draws - meanY * meanY, prob = hits / draws;
    double exactMean = Y0 + alpha * T + lambda * T * m;
    double exactVar = sigEff * sigEff * T + lambda * T * (s * s + m * m);
    // exact threshold probability: Poisson mixture of Gaussian tails
    double exactProb = 0, w = std::exp(-lambda * T);
    for (int n = 0; n < 200; ++n) {
        if (n > 0) w *= lambda * T / n;
        double sd = std::sqrt(sigEff * sigEff * T + n * s * s);
        exactProb += w * normalSF((std::log(Pc / (1 - Pc)) - Y0 - alpha * T - n * m) / sd);
    }
    std::cout << "Gamma = " << Gamma << " per year (focal class selected if > 0)\n";
    std::cout << "E[Y_1]       MC " << meanY << "   exact " << exactMean << "\n";
    std::cout << "Var[Y_1]     MC " << varY << "   exact " << exactVar << "\n";
    std::cout << "P(P_1>=0.3)  MC " << prob << " +/- " << 2 * std::sqrt(prob * (1 - prob) / draws)
              << "   exact " << exactProb << "\n";
    std::cout << "E[P_1]       MC " << sP / draws << "\n";

    // ---- 10 exact paths over 5 years: jumps placed at their exact random times ----
    double T5 = 5.0;
    int grid = 500, shown = 10;
    double dt = T5 / grid;
    std::poisson_distribution<int> K5(lambda * T5);
    std::vector<std::vector<double>> path(shown, std::vector<double>(grid + 1));
    for (int p = 0; p < shown; ++p) {
        int k = K5(gen);
        std::vector<double> jt(k), js(k);
        for (int j = 0; j < k; ++j) { jt[j] = T5 * U01(gen); js[j] = m + s * N01(gen); }
        double bm = 0;                                            // Brownian motion on the grid
        for (int n = 0; n <= grid; ++n) {
            double t = n * dt;
            if (n > 0) bm += std::sqrt(dt) * N01(gen);
            double y = Y0 + alpha * t + sigEff * bm;
            for (int j = 0; j < k; ++j) if (jt[j] <= t) y += js[j];
            path[p][n] = expit(y);
        }
    }
    std::ofstream out("data.csv");
    out << "t";
    for (int p = 1; p <= shown; ++p) out << ",path " << p;
    out << "\n";
    for (int n = 0; n <= grid; ++n) {
        out << n * dt;
        for (int p = 0; p < shown; ++p) out << "," << path[p][n];
        out << "\n";
    }
    return 0;
}
