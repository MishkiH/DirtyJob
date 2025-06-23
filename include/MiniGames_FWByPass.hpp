#ifndef MINIGAMES_FW_BYPASS_HPP_
#define MINIGAMES_FW_BYPASS_HPP_

#include "MiniGame.hpp"
#include "Player.hpp"

#include <string>
#include <vector>

class FWByPass : public MiniGame {
 public:
  explicit FWByPass(const std::string& maze_file);
  bool Play() override;
  std::string Name() const override { return "FWByPass"; }
  FWByPass(const std::string& maze_file, Player& player);


 private:
  Player& player_;
  struct MazeCell {
    char symbol;
  };

  std::string maze_file_;
  int rows_;
  int cols_;
  int player_x_;
  int player_y_;
  int exit_x_;
  int exit_y_;
  int max_moves_;
  int time_limit_;

  std::vector<std::vector<MazeCell>> maze_;

  void PrintMaze() const;
  void DrawBorder() const;
  bool MovePlayer(char dir);
};

#endif  // MINIGAMES_FW_BYPASS_HPP_
