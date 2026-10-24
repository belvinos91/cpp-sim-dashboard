// title: Change a variable inside a function
// category: Functions
// python:
//  def bump(state):        # lists are shared
//      state[0] += 1
// cpp:
//  void bump(std::vector<int>& state) {   // & = by reference
//      state[0] += 1;
//  }
// note: C++ copies arguments by default. `&` passes the original instead, so changes are kept; `const std::vector<double>&` passes the original read-only, without a copy (fast for big vectors).
// ---
#include <iostream>
#include <vector>

void bump_copy(std::vector<int> state) { state[0] += 1; }        // changes a copy
void bump_ref(std::vector<int>& state) { state[0] += 1; }        // changes the original

int main() {
    std::vector<int> s{0};
    bump_copy(s);
    std::cout << "after bump_copy: " << s[0] << "\n";   // 0
    bump_ref(s);
    std::cout << "after bump_ref:  " << s[0] << "\n";   // 1
    return 0;
}
