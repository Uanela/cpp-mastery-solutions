#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

struct Position {
  int number;
  char value;
};

bool is_valid_move(int move) { return move >= 1 && move <= 9; }

bool is_position_empty(int move, vector<Position> positions) {
  bool is = true;

  for (int i = 0; i < positions.size(); i++) {
    if (positions[i].value == move) {
      is = false;
      break;
    }
  }

  return is;
}

int main() {
  vector<vector<int>> win_combos = {
      {1, 2, 3}, {1, 5, 9}, {1, 4, 7}, {2, 5, 8},
      {3, 5, 7}, {3, 6, 9}, {4, 5, 6}, {7, 8, 9},
  };
  vector<Position> positions = {};
  char curr_player = 'x';
  int move;

  while (true) {
    cin >> move;

    if (is_valid_move(move)) {
      if (is_position_empty(move, positions)) {
      }
    } else {
      cout << "Please use a valid move!";
      break;
    }
  }

  for (int i = 0; i <= 100; ++i) {
    // \r moves the cursor to the start of the line
    std::cout << "\rRendering Frame: " << i << "%" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  // std::cout << std::endl;
  return 0;
}
