#include <chrono>
#include <iostream>
#include <thread>

class Timer {
private:
   std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

public:
   Timer() {
      start = std::chrono::high_resolution_clock::now();
   }

   ~Timer(){
      end = std::chrono::high_resolution_clock::now();
      auto duration = this->end - this->start;
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

      std::cout << "Elapsed time: " << ms.count()  << "ms";
   }
};

int main() {
   Timer t;

   std::this_thread::sleep_for(std::chrono::milliseconds(250));

   return 0;
}
