#include "MiniGames_FWByPass.hpp"
#include "EchoAI.hpp"
#include "../extern/nlohmann/json.hpp"
#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>
#include <conio.h>

using json = nlohmann::json;

FWByPass::FWByPass(const std::string& maze_file)
    : rows_(0), cols_(0), player_x_(0), player_y_(0), exit_x_(0), exit_y_(0) {
  LoadRandomMaze(maze_file);
}

void FWByPass::LoadRandomMaze(const std::string& maze_file) {
  std::ifstream file(maze_file);
  if (!file) {
    EchoAI::Instance().OnFail("Ошибка загрузки лабиринта");
    return;
  }
  json j;
  file >> j;
  // Случайный выбор лабиринта
  int maze_count = static_cast<int>(j.size());
  if (maze_count == 0) return;
  int idx = std::random_device{}() % maze_count;
  const auto& maze_obj = j[idx];

  max_moves_ = maze_obj.value("max_moves", 30);     // <- значение из json или 30
  time_limit_ = maze_obj.value("time_limit", 60);   // <- значение из json или 60

  const auto& maze_json = j[idx]["maze"];
  rows_ = static_cast<int>(maze_json.size());
  cols_ = static_cast<int>(maze_json[0].get<std::string>().size());

  maze_.clear();
  for (int y = 0; y < rows_; ++y) {
    std::string line = maze_json[y];
    std::vector<MazeCell> row;
    for (int x = 0; x < cols_; ++x) {
      char c = line[x];
      row.push_back({c});
      if (c == 'S') { player_x_ = x; player_y_ = y; }
      if (c == 'E') { exit_x_ = x; exit_y_ = y; }
    }
    maze_.push_back(row);
  }
}

void FWByPass::PrintMaze() const {
  DrawBorder();
  for (int y = 0; y < rows_; ++y) {
    std::cout << "|";
    for (int x = 0; x < cols_; ++x) {
      if (x == player_x_ && y == player_y_) {
        std::cout << "\033[32m@\033[0m";  // игрок
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
  int nx = player_x_, ny = player_y_;
  switch (dir) {
    case 'w': --ny; break;
    case 'a': --nx; break;
    case 's': ++ny; break;
    case 'd': ++nx; break;
    default: return false;
  }
  if (nx < 0 || ny < 0 || nx >= cols_ || ny >= rows_) return false;
  if (maze_[ny][nx].symbol == '#') return false;
  player_x_ = nx; player_y_ = ny;
  return true;
}

// Логика самой мини-игры (упрощённая, без фаз зоны)
bool FWByPass::Play() {
  EchoAI::Instance().OnMiniGameStart(Name());
  auto time_start = std::chrono::steady_clock::now();
  bool win = false;
  int max_moves = 30;
  int moves = 0;
  PrintMaze();
  std::cout << "Управление: WASD. Выйди к E (exit). Ограничение по ходам!\n";
  while (moves < max_moves_) {
    // вычисляем оставшееся время на каждой итерации
    auto time_now = std::chrono::steady_clock::now();
    int seconds_left = time_limit_ - int(std::chrono::duration_cast<std::chrono::seconds>(time_now - time_start).count());

    if (seconds_left <= 0) {
        std::cout << "Время вышло!" << '\n';
        break;
    }

    Utils::ClearScreen();
    std::cout << "Осталось ходов: " << (max_moves_ - moves)
              << "   Времени: " << seconds_left << " сек." << '\n';
    PrintMaze();

    std::cout << "Твой ход (WASD): ";

    char ch = _getch();

    std::cout << ch << '\n';

    if (MovePlayer(ch)) {
        ++moves;
        if (player_x_ == exit_x_ && player_y_ == exit_y_) {
            win = true;
            break;
        }
    } else {
        EchoAI::Instance().OnFail("Ход невозможен");
    }
}
  EchoAI::Instance().OnMiniGameEnd(win);
  return win;
}
