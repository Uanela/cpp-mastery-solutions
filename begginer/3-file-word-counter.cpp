#include <cctype>
#include <iostream>
#include <fstream>

using namespace std;

bool is_special(char c) {
  return c == ' ' || !isalnum(static_cast<unsigned char>(c));
}

int main() {
  string file_path = "../README.md";
  ifstream file(file_path);

  string line;
  int line_counter = 0;
  int word_counter = 0;

  while (getline(file, line)) {
    line_counter++;
    for (int i = 0; i < line.length(); i++) {
      if (i > 0 && ( is_special(line[i]) || i == line.length() - 1 ) && !is_special(line[i-1])) word_counter++;
    }
  }

  cout << "The file at " << file_path << endl;
  cout << "Has " << word_counter << " words" << endl;
  cout << "Has " << line_counter << " lines" << endl;

  return 0;
}
