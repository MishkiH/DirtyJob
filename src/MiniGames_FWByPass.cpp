#include "MiniGames_FWByPass.hpp"
#include "EchoAI.hpp"
#include "Utils.hpp"
#include "../extern/nlohmann/json.hpp"

#include <atomic>
#include <chrono>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using json = nlohmann::json;

<<<<<<< HEAD
FWByPass::FWByPass(const std::string& maze_file, Player& player)
    : rows_(0), cols_(0), player_x_(0), player_y_(0), exit_x_(0), exit_y_(0),
    max_moves_(30), time_limit_(60), player_(player) {};


=======
FWByPass::FWByPass(const std::string& maze_file)
    : maze_file_(maze_file),
      rows_(0),
      cols_(0),
      player_x_(0),
      player_y_(0),
      exit_x_(0),
      exit_y_(0) {}
>>>>>>> e63cd5999c7d5153852048af49b10ec1c75fcac1

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
<<<<<<< HEAD
    EchoAI::Instance().OnMiniGameStart(Name());
    bool win = false;
    int moves = 0;
    int max_moves = max_moves_;
    int time_limit = time_limit_;

    bool coffee = player_.HasCoffee();
    if (coffee) {
        // Если активирован кофе, увеличиваем количество ходов и время
        max_moves += 5;
        time_limit += 20;
        EchoAI::Instance().OnSuccess("Coffee.exe активен! У тебя больше времени и ходов на миссию.");
    }

    std::vector<json> all_mazes;
    int level = 0;
    std::cout << "Размер all_mazes: " << all_mazes.size() << std::endl;
    std::cout << "Текущий уровень: " << level << std::endl;
    if (level >= all_mazes.size()) {
        std::cerr << "Ошибка: Уровень выходит за пределы!" << std::endl;
        return false;  // Прерываем выполнение, если уровень некорректен
    }

    const auto& maze_obj = all_mazes[level];  // Получаем лабиринт по индексу
    max_moves_ = maze_obj.value("max_moves", 30);
    time_limit_ = maze_obj.value("time_limit", 60);

    const auto& maze_json = maze_obj["maze"];
    rows_ = static_cast<int>(maze_json.size());
    cols_ = static_cast<int>(maze_json[0].get<std::string>().size());

    maze_.clear();  // Очистить перед новым заполнением
    for (int y = 0; y < rows_; ++y) {
        std::string line = maze_json[y];
        if (line.size() != cols_) {
            std::cout << "Warning: line size does not match cols_" << std::endl;
            continue;  // Пропускаем строку, если её размер не совпадает с ожидаемым
        }

        std::vector<MazeCell> row;
        for (int x = 0; x < cols_; ++x) {
            if (x >= line.size()) {
                std::cout << "Error: out of range for line " << y << ", index " << x << std::endl;
                continue;
            }
            char c = line[x];
            row.push_back({ c });
            if (c == 'S') { player_x_ = x; player_y_ = y; }
            if (c == 'E') { exit_x_ = x; exit_y_ = y; }
        }
        maze_.push_back(row);
    }
}





=======
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
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
        std::cout << "Уровень " << (level + 1) << " из " << kTotalLevels << "\n";
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
      std::this_thread::sleep_for(std::chrono::seconds(2));
      Utils::ClearScreen();
    }
  }

  EchoAI::Instance().OnMiniGameEnd(overall_win);
  return overall_win;
}
>>>>>>> e63cd5999c7d5153852048af49b10ec1c75fcac1
