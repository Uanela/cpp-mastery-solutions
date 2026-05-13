#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

enum class LogCategory { LOG, INFO, ERROR, WARN, DONE };

class Logger {
private:
  void write(LogCategory c, string &t) {
    ofstream file(filepath, ios::app);

    if (file.is_open()) {
      file << get_timestamp() << " [" << get_category(c) << "] " << t << endl;

      file.close();
    } else {
      cout << "Couldn't write to log to file at " << filepath << endl;
    }
  }

  basic_string<char> get_timestamp() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);

    stringstream ss;
    ss << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
  }

  string get_category(LogCategory &c) {
    switch (c) {
      case LogCategory::LOG: return "Log";
      case LogCategory::INFO: return "Info";
      case LogCategory::ERROR: return "Error";
      case LogCategory::WARN: return "Warn";
      case LogCategory::DONE: return "Done";
      default: return "Log";
    }
  }
public:
  string filepath;

  Logger(string f): filepath(f) { }

  void log(string &t) {
    write(LogCategory::LOG, t);
  }
  void info(string t) {
    write(LogCategory::INFO, t);
  }
  void error(string t) {
    write(LogCategory::ERROR, t);
  }
  void warn(string t){
    write(LogCategory::WARN, t);
  } 
  void done(string t) {
    write(LogCategory::DONE, t);
  }
};

int main() { 
  Logger console("../.cache/logs.txt");
  string line;
  
  cout << "Let's log some files\n";

  while(getline(cin, line)){
    console.info(line);
  }

  return 0;
}
