#include <iostream>

using namespace std;

enum PlayerType { x, o };

struct Position {
  int move;
  PlayerType player;
};


vector<vector<int>> win_combos = {
    {1, 2, 3}, {1, 5, 9}, {1, 4, 7}, {2, 5, 8},
    {3, 5, 7}, {3, 6, 9}, {4, 5, 6}, {7, 8, 9},
};


bool is_valid_move(int move) { return move >= 1 && move <= 9; }

bool is_position_empty(int move, vector<Position> positions) {
  bool is = true;

  for (int i = 0; i < positions.size(); i++) {
    if (positions[i].move == move) {
      is = false;
      break;
    }
  }

  return is;
}


PlayerType get_player_at_position(int pos, vector<Position> positions) {
  for (int i = 0; i < positions.size(); i++) {
    if (positions[i].move == pos) return positions[i].player;
  }
  throw runtime_error("pos not found");
}


vector<int> get_player_moves(PlayerType player, vector<Position> positions) { 
  vector<int> player_moves = {};

  for (int i = 0; i < positions.size(); i++) {
    if (positions[i].player == player) player_moves.push_back(positions[i].move);
  }

  return player_moves;
}


template <typename T>
bool vector_includes(const vector<T>& vec,const T& value) {
  auto it = find(vec.begin(), vec.end(), value);

  if (it != vec.end()) return true;
  else return false;
}

bool player_won(PlayerType player, vector<Position> positions) {
  vector<int> player_moves = get_player_moves(player, positions);
  int counter = 0;
  bool won = false;

  for (int i = 0; i < win_combos.size(); i++){
    int first = win_combos[i][0];
    int second = win_combos[i][1];
    int third = win_combos[i][2];

    if (vector_includes(player_moves, first) && vector_includes(player_moves, second) && vector_includes(player_moves, third) ) {
won = true;
break;
    }
  }

  return won;
}

char get_player_icon(PlayerType player_type) {
  return player_type == PlayerType::o ? 'o' : 'x';
}

void redraw(vector<Position> positions, PlayerType curr_player, bool preserve = false) {
  cout << "Player " << get_player_icon(curr_player) << " turn\n\n" << flush;
  for (int i = 0; i < 9; i++) {
    char cell = !is_position_empty(i+1, positions) ? get_player_icon(get_player_at_position(i+1, positions)) : ' ';
    cout << " " << cell  << (( i+1 ) % 3 == 0 ? "\n" : "|");
  }

  if (!preserve)
    cout << "\r\033[6A";
  else cout << "\n";
}

int main() {
  cout << "Tic Tac Toe\n\n";
  vector<Position> positions = {};
  PlayerType curr_player = PlayerType::x;
  int move;

  redraw(positions, curr_player);
  while (true) {


    cin >> move;

    if (is_valid_move(move)) {
      if (is_position_empty(move, positions)) {
        positions.push_back({.move = move, .player = curr_player});

        if (player_won(curr_player, positions)) {
          redraw(positions, curr_player, true);
          cout << "Player " << get_player_icon(curr_player) << " won!\n";
          break;
        }

        curr_player = curr_player == PlayerType::x ? PlayerType::o : PlayerType::x;

        


    if (positions.size() == 9) {
      redraw(positions, curr_player, true);
      cout << "Game finished with a draw!";
      break;
    } else  redraw(positions, curr_player);

      } else
        throw runtime_error( "The position you are trying to play is not empty\n" );
    } else
      throw runtime_error( "Please use a valid move!\n" );
  }

  return 0;
}
