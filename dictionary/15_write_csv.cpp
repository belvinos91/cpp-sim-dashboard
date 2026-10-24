// title: Write a CSV file
// category: Files
// python:
//  with open("data.csv", "w") as f:
//      f.write("t,x\n")
//      for t, x in rows:
//          f.write(f"{t},{x}\n")
// cpp:
//  std::ofstream out("data.csv");
//  out << "t,x\n";
//  for (...) out << t << "," << x << "\n";
// note: `std::ofstream` works like `std::cout` but writes to a file, which closes itself at the end of `main`. In this dashboard, a file named `data.csv` is plotted automatically.
// ---
#include <cmath>
#include <fstream>

int main() {
    std::ofstream out("data.csv");
    out << "t,sin,cos\n";
    for (int i = 0; i <= 100; ++i) {
        double t = i * 0.1;
        out << t << "," << std::sin(t) << "," << std::cos(t) << "\n";
    }
    return 0;
}
