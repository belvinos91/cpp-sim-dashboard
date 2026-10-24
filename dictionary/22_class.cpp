// title: A class with a constructor and methods
// category: Classes
// python:
//  class Model:
//      def __init__(self, beta, gamma):
//          self.beta = beta
//          self.gamma = gamma
//      def R0(self):
//          return self.beta / self.gamma
// cpp:
//  class Model {
//  public:
//      Model(double beta, double gamma)
//          : beta_(beta), gamma_(gamma) {}
//      double R0() const { return beta_ / gamma_; }
//  private:
//      double beta_, gamma_;
//  };
// note: The constructor (same name as the class) replaces `__init__`; there is no `self`. `const` after a method promises it does not change the object. `private` members can only be used inside the class.
// ---
#include <iostream>

class Model {
public:
    Model(double beta, double gamma) : beta_(beta), gamma_(gamma) {}
    double R0() const { return beta_ / gamma_; }
private:
    double beta_, gamma_;
};

int main() {
    Model m(0.3, 0.1);
    std::cout << "R0 = " << m.R0() << "\n";
    return 0;
}
