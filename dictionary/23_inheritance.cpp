// title: Inheritance and virtual methods (the Effect pattern)
// category: Classes
// python:
//  class Intervention:
//      def apply(self, state): ...
//  
//  class Screen(Intervention):
//      def apply(self, state):
//          state["I"] -= 1
// cpp:
//  struct Intervention {
//      virtual void apply(State& s) const = 0;
//      virtual ~Intervention() = default;
//  };
//  struct Screen : Intervention {
//      void apply(State& s) const override { s.I -= 1; }
//  };
// note: `virtual ... = 0` declares a method every subclass must write (like an abstract method). A list of `std::unique_ptr<Intervention>` can hold any mix of subclasses: the simulation loop calls `apply` without knowing which intervention it is, so new interventions never change the loop.
// ---
#include <iostream>
#include <memory>
#include <vector>

struct State { int S, I; };

struct Intervention {
    virtual void apply(State& s) const = 0;
    virtual ~Intervention() = default;
};
struct Screen : Intervention {
    void apply(State& s) const override { s.I = s.I / 2; }          // treat half of the infected
};
struct Vaccinate : Intervention {
    void apply(State& s) const override { s.S -= s.S / 10; }        // vaccinate 10% of susceptibles
};

int main() {
    State s{900, 100};
    std::vector<std::unique_ptr<Intervention>> plan;
    plan.push_back(std::make_unique<Screen>());
    plan.push_back(std::make_unique<Vaccinate>());
    for (const auto& step : plan) step->apply(s);                    // the loop never changes
    std::cout << "S = " << s.S << ", I = " << s.I << "\n";
    return 0;
}
