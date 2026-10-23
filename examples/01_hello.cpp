// Hello: reads optional command-line arguments and prints them.
// Try typing  2 40  in the Arguments box.
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Hello from C++!\n";
    std::cout << "You passed " << argc - 1 << " argument(s):\n";
    for (int i = 1; i < argc; ++i) std::cout << "  " << argv[i] << "\n";
    return 0;
}
