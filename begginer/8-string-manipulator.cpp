#include <iostream>

class String {

  public:
    std::string data;
    // tells the compiler to create a default constructor
    // so that the class can be instatiated without arguments
    String() = default;

    String(const char *str) : data(str) {}
    String(const std::string &str) : data(str) {}

    String& operator=(const char *str) {
      data = str;
      std::string a = "u";
      return *this;
    }

    std::vector<char> split() {
      std::vector<char> chars = {};

      for (char c : this->data)
      {
        chars.push_back(c);
      }
      return chars;
    }

    String slice(int s = 0, int e = 0) {
      
    }
};

std::ostream& operator<<(std::ostream& os, const String& obj) {
    os << obj.data;
    return os;
}

int main() {
  String name = "Uanela";

  std::cout << name << std::endl;

  std::cout << name.split().size();

  return 0;
}
