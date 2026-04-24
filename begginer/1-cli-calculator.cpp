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

double calc(double num1, double num2, char op) {
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
bool is_special(char op) { return op == '*' || op == '/'; }

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
  pair<string, char> pending;

  for (int i = 0; i < expression.length(); i++) {
    char c = expression[i];

    if (is_operator(c))
      operators.push_back(c);
    else if (is_operator(last_char))
      operands.push_back(string{c});
    else if (operands.size() > 0)
      operands[operands.size() - 1] =
          (operands[operands.size() - 1]) + string{c};
    else
      operands.push_back(string{c});

    last_char = c;
  }

  double result = stod(operands[0]);
  double* tmp_operand = nullptr;
  char* tmp_operator = nullptr;

  for (int i = 0; i < operands.size() - 1; i++) {
    char curr_operator = operators[i];
    char next_operator_i = i + 1;
    char next_operator = operators[next_operator_i];
    char after_next_operator = operators[next_operator_i + 1];

    double curr_operand = stod( operands[i] );
    double next_operand = stod( operands[next_operator_i] );

    if (!is_special(next_operator)) {
      result = calc(result, next_operand, curr_operator);
      if (tmp_operand != nullptr && tmp_operator != nullptr) {
        result = calc(*tmp_operand, result, *tmp_operator);
        tmp_operator = nullptr;
        tmp_operand = nullptr;
      }
    } else {
      double after_next_operand = stod(operands[next_operator_i + 1]);
      double tmp = calc(next_operand, after_next_operand, next_operator);

      if (!is_special(after_next_operator))
        result = calc(result, tmp, curr_operator);
      else if (i + 2 < operands.size()) {
        tmp_operand = new double(result);
        tmp_operator = new char(curr_operator);
        result = tmp;
      }
      i++;
    }
  }

  cout << expression << " = " << result;



  return 0;
}
