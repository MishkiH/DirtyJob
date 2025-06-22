#pragma once

#include "MiniGame.hpp"
#include <vector>
#include <string>

class FWByPass : public MiniGame {
public:
    explicit FWByPass(const std::string& maze_file);
    bool Play() override;
    std::string Name() const override { return "FWByPass"; }

private:
    struct MazeCell {
        char symbol;
    };

    std::string maze_file_;          // <-- путь к JSON с лабиринтами
    int rows_, cols_;
    int player_x_, player_y_;
    int exit_x_, exit_y_;
    int max_moves_;
    int time_limit_;

    std::vector<std::vector<MazeCell>> maze_;

    void PrintMaze() const;
    void DrawBorder() const;
    bool MovePlayer(char dir);
};
