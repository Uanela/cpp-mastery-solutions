#include <iostream>
#include "json.h"

struct JsonValue;
using JsonObject = std::unordered_map<std::string, JsonValue>;
using JsonArray =  std::vector<JsonValue>;

JsonValue::JsonValue() : data(std::monostate{}) {}
JsonValue::JsonValue(bool v) : data(v) {}
JsonValue::JsonValue(int v) : data(v) {}
JsonValue::JsonValue(double v) : data(v) {}
JsonValue::JsonValue(const char* v) : data(std::string(v)) {}
JsonValue::JsonValue(std::string v) : data(std::move(v)) {}
JsonValue::JsonValue(JsonArray v) : data(std::move(v)) {}
JsonValue::JsonValue(JsonObject v) : data(std::move(v)) {}

std::string JsonValue::to_string() {
  std::string str = "";
  auto v = this->data;


  
  return str;
} 

