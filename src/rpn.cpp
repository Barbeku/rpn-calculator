#include "rpn.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>

using namespace std;

string toBinary(int number) {
  if (number == 0) return "0";
  
  std::stack<char> binaryStack;
  bool isNegative = number < 0;
  unsigned int n = isNegative ? -number : number;
  
  while (n > 0) {
    binaryStack.push((n % 2) + '0');
    n /= 2;
  }
  
  std::string result;
  if (isNegative) {
    result += '-';
  }
  
  while (!binaryStack.empty()) {
    result += binaryStack.top();
    binaryStack.pop();
  }
  
  return result;
}

int factorial(int a){
  int product = a;
  while(--a)
    product *= a;
  return product;
}

vector<string> tokenize(const string& expression) {
  vector<string> tokens;
  istringstream iss(expression);
  string token;
  
  while (iss >> token) {
    tokens.push_back(token);
  }
  
  return tokens;
}

bool is_number(const string& s) {
  if (s.empty()) return false;
  
  size_t start = 0;
  if (s[0] == '-' || s[0] == '+') {
    if (s.size() == 1) return false;
    start = 1;
  }
  
  bool has_dot = false;
  for (size_t i = start; i < s.size(); ++i) {
    if (s[i] == '.') {
      if (has_dot) return false;
      has_dot = true;
    }
    else if (!isdigit(s[i])) {
      return false;
    }
  }
  return true;
}

double evaluate_rpn(const string& expression) {
  srand(time(NULL));

  stack<double> stackC;
  vector<string> tokens = tokenize(expression);
  
  for (const auto& token : tokens) {
    if (is_number(token)) {
      stackC.push(stod(token));
    }
    else {
      if(token == "clear"){
        while(stackC.size() != 0)
          stackC.pop();
        continue;
      }
      else if(token == "stack"){
        stack<double> tempStack = stackC;
        stack<double> reverseStack;
        
        while (!tempStack.empty()) {
          reverseStack.push(tempStack.top());
          tempStack.pop();
        }
      
        while (!reverseStack.empty()) {
          cout << reverseStack.top() << " ";
          reverseStack.pop();
        }
        cout << endl;
        continue;
      }
      else if(token == "pi"){
        stackC.push(M_PI);
        continue;
      }
      else if(token == "e"){
        stackC.push(exp(1));
        continue;
      }
      else if(token == "rand"){
        stackC.push(rand() % 2);
        continue;
      }

      if(token == "~" || token == "++" || token == "--" || token == "!" || token == "dup" || token == "drop" || token == "sqrt" || token == "abs" || token == "sin" || token == "cos" || token == "tan" || token == "asin" || token == "acos" || token == "atan" || token == "ln" || token == "log" || token == "floor" || token == "ceil" || token == "round" || token == "bin" || token == "hex")
        ;
        //all right
      else if (stackC.size() < 2)
        throw invalid_argument("Not enough operands for operator '" + token + "'");

      double result;
      double b = stackC.top(); stackC.pop();

      if(token == "~")
        result = -b;
      else if(token == "++")
        result = b + 1;
      else if(token == "--")
        result = b - 1;
      else if(token == "!")
        result = factorial(b);
      else if(token == "dup"){
        result = b;
        stackC.push(result);
      }
      else if(token == "sqrt") result = sqrt(b);
      else if(token == "abs") result = abs(b);
      else if(token == "sin") result = sin(b);
      else if(token == "cos") result = cos(b);
      else if(token == "tan") result = tan(b);
      else if(token == "asin") result = asin(b);
      else if(token == "acos") result = acos(b);
      else if(token == "atan") result = atan(b);
      else if(token == "ln") result = log(b);
      else if(token == "log") result = log10(b);
      else if(token == "floor") result = floor(b);
      else if(token == "ceil") result = ceil(b);
      else if(token == "round") result = round(b);
      else if(token == "bin"){
        result =  b;
        cout << toBinary(b) << endl;
      }
      else if(token == "hex"){
        result = b;
        printf("%x\n", (unsigned int)b);
      }
      else{
        //binary operation
        double a = stackC.top(); stackC.pop();
        
        if (token == "+")
          result = a + b;
        else if (token == "-")
          result = a - b;
        else if (token == "*")
          result = a * b;
        else if (token == "/") {
          if (b == 0.0)
            throw invalid_argument("Division by zero");
          result = a / b;
        }
        else if (token == "^")
          result = pow(a, b);
        else if (token == "%")
          result = (int)a % (int)b;
        else if(token == "swap"){
          stackC.push(b);
          result = a;
        }
        else if(token == "==") result = a == b;
        else if(token == "!=") result = a != b;
        else if(token == ">") result = a > b;
        else if(token == "<") result = a < b;
        else if(token == ">=") result = a >= b;
        else if(token == "<=") result = a <= b;
        else
          throw invalid_argument("Invalid operator: '" + token + "'");
      }
      
      stackC.push(result);
    }
  }
  
  if (stackC.size() > 1) {
    throw invalid_argument("Invalid expression: too many operands");
  }
  
  return stackC.size() == 1 ? stackC.top() : -1;
}
