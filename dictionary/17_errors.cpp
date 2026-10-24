// title: Handle errors: try / except
// category: Errors
// python:
//  try:
//      n = int("12abc")
//  except ValueError:
//      print("not a number")
// cpp:
//  try {
//      int n = std::stoi("abc");
//  } catch (const std::exception& e) {
//      std::cout << "not a number\n";
//  }
// note: Same structure as Python. Careful: `std::stoi("12abc")` returns 12 without an error; check how many characters were read (the `add.cpp` trick).
// ---
#include <iostream>
#include <string>

int main() {
    for (std::string s : {"42", "abc", "12abc"}) {
        try {
            std::size_t used = 0;
            int n = std::stoi(s, &used);
            if (used != s.size()) std::cout << s << ": only partly a number (" << n << ")\n";
            else std::cout << s << ": ok, " << n << "\n";
        } catch (const std::exception& e) {
            std::cout << s << ": not a number\n";
        }
    }
    return 0;
}
