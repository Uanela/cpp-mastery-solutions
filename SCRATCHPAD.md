#include <iostream>
#include <unordered_map>
#include <variant>
#include <string>
#include <vector>

struct JsonValue;

using JsonObject = std::unordered_map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

struct JsonValue {
std::variant<
std::monostate, // null
bool,
int,
double,
std::string,
JsonArray,
JsonObject > data;

    JsonValue() : data(std::monostate{}) {}
    JsonValue(bool v)              : data(v) {}
    JsonValue(int v)               : data(v) {}
    JsonValue(double v)            : data(v) {}
    JsonValue(const char* v)       : data(std::string(v)) {}
    JsonValue(std::string v)       : data(std::move(v)) {}
    JsonValue(JsonArray v)         : data(std::move(v)) {}
    JsonValue(JsonObject v)        : data(std::move(v)) {}

};

int main() {
JsonObject json;
json["status"] = 200;
json["message"] = "OK";
json["success"] = true;

    JsonObject server;
    server["ip"]   = "127.0.0.1";
    server["port"] = 8080;
    json["server"] = server;

    int         status = std::get<int>        (json["status"].data);
    std::string msg    = std::get<std::string>(json["message"].data);
    JsonObject  nested = std::get<JsonObject> (json["server"].data);
    std::string ip     = std::get<std::string>(nested["ip"].data);

    std::cout << "Response: " << status << " " << msg << " from " << ip << "\n";
    // Response: 200 OK from 127.0.0.1

}
