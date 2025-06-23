#ifndef MINIGAMES_FW_BYPASS_HPP_
#define MINIGAMES_FW_BYPASS_HPP_

#include "MiniGame.hpp"
<<<<<<< HEAD
#include "Player.hpp"
=======
>>>>>>> e63cd5999c7d5153852048af49b10ec1c75fcac1

#include <string>
#include <vector>

class FWByPass : public MiniGame {
 public:
  explicit FWByPass(const std::string& maze_file);
  bool Play() override;
  std::string Name() const override { return "FWByPass"; }
<<<<<<< HEAD
  FWByPass(const std::string& maze_file, Player& player);


 private:
  Player& player_;
=======

 private:
>>>>>>> e63cd5999c7d5153852048af49b10ec1c75fcac1
  struct MazeCell {
    char symbol;
  };

<<<<<<< HEAD
  std::string maze_file_;
=======
  std::string maze_file_;  // jsonчик
>>>>>>> e63cd5999c7d5153852048af49b10ec1c75fcac1
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
