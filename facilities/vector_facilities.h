#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <optional>
using namespace std;

string join(vector<char> arr, string delimeter = ", ") {
  string result = "";
  for (int i = 0; i < arr.size(); i++) {
    if (result.length() > 0)
      result = result + delimeter + arr[i];
    else result = arr[i];
  }
  return result;
}

string join(vector<string> arr, string delimeter = ", ") {
  string result = "";
  for (int i = 0; i < arr.size(); i++) {
    if (result.length() > 0)
      result = result + delimeter + (arr[i]);
    else result = arr[i];
  }
  return result;
}

string join(vector<double> arr, string delimeter = ", ") {
  string result = "";
  for (int i = 0; i < arr.size(); i++) {
    if (result.length() > 0)
      result = result + delimeter + to_string(arr[i]);
    else result = to_string(arr[i]);
  }
  return result;
}
