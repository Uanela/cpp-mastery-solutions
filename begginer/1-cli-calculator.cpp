#include <iostream>

using namespace std;

bool is_operator(char c) {
  bool is = true;

  switch (c) {
    case '+':
    case '-':
    case '/':
    case '*':
      break;
    default:
      is = false;
      break;
  }

  return is;
}

double calc(double num1, double num2, char op){
  switch (op) {
    case '+':
      return num1 + num2;
    case '-':
      return num1 - num2;
    case '*':
      return num1 * num2;
    case '/':
      return num1 / num2;
    default:
      return 0;
  }
}

// double handle_multi_and_div(double curr, double num1, double num2, char op) {

// }

int main() {
  string expression;
  string input;

  while (true) {
    getline(cin, input);

    if (input.empty()) {
      if (expression.empty())
        cout << "No expression was provided";
      break;
    }
    expression += input;
  }

  vector<char> operators = {};
  vector<string> operands = {};
  char last_char = 'a';

  for (int i = 0; i < expression.length(); i++) {
    char c = expression[i];

    if (is_operator(c))
      operators.push_back(c);
    else if (is_operator(last_char))
      operands.push_back(string{c});
    else if(operands.size() > 0)
      operands[operands.size() - 1] = (operands[operands.size() - 1]) + string{c};
    else 
       operands.push_back(string{c});

    last_char = c;
  }

  double result = stod(operands[0]);
  for (int i = 0; i < operands.size() - 1; i = i+1) {
    if (operators[i + 1] != '*' && operators[i + 1] != '/')
    {
      if (i == 0 || ( operators[i] != '*' && operators[i] != '/' ))
        result = calc(result, stod(operands[i+1]), operators[i]);
    }
    else
    {
      double tmp = calc(stod(operands[i+1]), stod(operands[i+2]), operators[i + 1]);
      result = calc(result, tmp, operators[i]);
      i = i+1;
    }
  }

  cout << expression << " = " << result;

  return 0;
}

