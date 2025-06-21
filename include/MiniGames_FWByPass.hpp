#ifndef MINIGAMES_FWBYPASS_HPP_
#define MINIGAMES_FWBYPASS_HPP_

#include "MiniGame.hpp"
#include <vector>
#include <string>

struct MazeCell {
  char symbol;  // ' ', '#', 'S', 'E'
};

class FWByPass : public MiniGame {
 public:
  FWByPass(const std::string& maze_file);

  bool Play() override;
  std::string Name() const override { return "FWByPass"; }

 private:
  int max_moves_ = 30;
  int time_limit_ = 30;
  std::vector<std::vector<MazeCell>> maze_;
  int rows_, cols_;
  int player_x_, player_y_;
  int exit_x_, exit_y_;
  std::vector<std::string> maze_lines_;

  void LoadRandomMaze(const std::string& maze_file);
  void PrintMaze() const;
  bool MovePlayer(char dir);
  void DrawBorder() const;
};

#endif  // MINIGAMES_FWBYPASS_HPP_
