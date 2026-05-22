#pragma once

#include <iostream>

struct JsonValue;
using JsonObject = std::unordered_map<std::string, JsonValue>;
using JsonArray =  std::vector<JsonValue>;

struct JsonValue {
  std::variant<
    std::monostate, // null
    bool,
    int,
    double,
    std::string,
    JsonArray,
    JsonObject
  > data;

  JsonValue(); 
  JsonValue(bool v);
  JsonValue(int v);
  JsonValue(double v);
  JsonValue(const char* v);
  JsonValue(std::string v);
  JsonValue(JsonArray v);
  JsonValue(JsonObject v);

  std::string to_string();
};


