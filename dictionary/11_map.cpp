// title: Dictionary -> std::map
// category: Containers
// python:
//  params = {"beta": 0.3, "gamma": 0.1}
//  print(params["beta"])
// cpp:
//  std::map<std::string, double> params{
//      {"beta", 0.3}, {"gamma", 0.1}};
//  std::cout << params["beta"];
// note: `std::map` keeps keys sorted; `std::unordered_map` is the faster, unsorted one, closer to a Python `dict`. For model parameters, a `struct` (next entry) is usually clearer.
// ---
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, double> params{{"beta", 0.3}, {"gamma", 0.1}};
    for (const auto& [name, value] : params) {
        std::cout << name << " = " << value << "\n";
    }
    std::cout << "R0 = " << params["beta"] / params["gamma"] << "\n";
    return 0;
}
