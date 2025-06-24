#include "MiniGames_FWByPass.hpp"

using json = nlohmann::json;

FWByPass::FWByPass(const std::string& maze_file)
    : maze_file_(maze_file),
      rows_(0),
      cols_(0),
      player_x_(0),
      player_y_(0),
      exit_x_(0),
      exit_y_(0) {}

void FWByPass::PrintMaze() const {
  DrawBorder();
  for (int y = 0; y < rows_; ++y) {
    std::cout << "|";
    for (int x = 0; x < cols_; ++x) {
      if (x == player_x_ && y == player_y_) {
        std::cout << "\033[32m@\033[0m";
      } else if (maze_[y][x].symbol == '#') {
        std::cout << "\033[37m#\033[0m";
      } else if (maze_[y][x].symbol == 'E') {
        std::cout << "\033[31mE\033[0m";
      } else if (maze_[y][x].symbol == 'S') {
        std::cout << "S";
      } else {
        std::cout << " ";
      }
    }
    std::cout << "|\n";
  }
  DrawBorder();
}

void FWByPass::DrawBorder() const {
  std::cout << "+";
  for (int i = 0; i < cols_; ++i) std::cout << "-";
  std::cout << "+\n";
}

bool FWByPass::MovePlayer(char dir) {
  int nx = player_x_;
  int ny = player_y_;
  switch (dir) {
    case 'w':
      --ny;
      break;
    case 'a':
      --nx;
      break;
    case 's':
      ++ny;
      break;
    case 'd':
      ++nx;
      break;
    default:
      return false;
  }
  if (nx < 0 || ny < 0 || nx >= cols_ || ny >= rows_) return false;
  if (maze_[ny][nx].symbol == '#') return false;
  player_x_ = nx;
  player_y_ = ny;
  return true;
}

bool FWByPass::Play() {
  EchoAI::Instance().OnMiniGameStart(Name());
  bool overall_win = true;
  const int kTotalLevels = 3;

  std::ifstream file(maze_file_);
  if (!file) {
    EchoAI::Instance().OnFail("Не удалось загрузить файл лабиринтов");
    return false;
  }
  json all_mazes;
  file >> all_mazes;

  for (int level = 0; level < kTotalLevels; ++level) {
    const auto& maze_obj = all_mazes[level];
    max_moves_ = maze_obj.value("max_moves", 30);
    time_limit_ = maze_obj.value("time_limit", 60);
    const auto& maze_json = maze_obj["maze"];

    rows_ = static_cast<int>(maze_json.size());
    cols_ = static_cast<int>(maze_json[0].get<std::string>().size());

    maze_.clear();
    for (int y = 0; y < rows_; ++y) {
      std::string line = maze_json[y];
      std::vector<MazeCell> row;
      for (int x = 0; x < cols_; ++x) {
        char c = line[x];
        row.push_back({c});
        if (c == 'S') {
          player_x_ = x;
          player_y_ = y;
        }
        if (c == 'E') {
          exit_x_ = x;
          exit_y_ = y;
        }
      }
      maze_.push_back(row);
    }

    int moves = 0;
    std::atomic<int> seconds_left(time_limit_);
    std::atomic<bool> running(true);
    bool win = false;

    Utils::ClearScreen();
    std::cout << "Уровень " << (level + 1) << " из " << kTotalLevels << "\n";
    std::cout << "Осталось ходов: " << (max_moves_ - moves)
              << "   Времени: " << seconds_left << " сек.\n";
    PrintMaze();
    std::cout << "Твой ход (WASD):\n";

    std::thread timer_thread([&]() {
      while (running && seconds_left > 0) {
        Utils::SleepFor(1000);
        --seconds_left;
        std::cout << "\033[2;1H";
        std::cout << "Осталось ходов: " << (max_moves_ - moves)
                  << "   Времени: " << seconds_left << " сек.   \n";
        std::cout.flush();
      }
      running = false;
    });

    while (running) {
      if (_kbhit()) {
        char ch = _getch();
        if (MovePlayer(ch)) {
          ++moves;
          if (player_x_ == exit_x_ && player_y_ == exit_y_) {
            win = true;
            running = false;
          }
        }

        Utils::ClearScreen();
        std::cout << "Уровень " << (level + 1) << " из " << kTotalLevels
                  << "\n";
        std::cout << "Осталось ходов: " << (max_moves_ - moves)
                  << "   Времени: " << seconds_left << " сек.\n";
        PrintMaze();
        std::cout << "Твой ход (WASD):\n";

        if (moves >= max_moves_) {
          running = false;
        }
      }
    }

    if (timer_thread.joinable()) timer_thread.join();

    Utils::ClearScreen();
    if (!win) {
      if (seconds_left <= 0)
        std::cout << "Время вышло!\n";
      else
        std::cout << "Ходы закончились!\n";
      overall_win = false;
      break;
    } else {
      std::cout << "Уровень " << (level + 1) << " пройден!\n";
      Utils::SleepFor(2000);
      Utils::ClearScreen();
    }
  }

  EchoAI::Instance().OnMiniGameEnd(overall_win);
  return overall_win;
}
