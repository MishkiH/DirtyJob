#ifndef MINIGAME_HPP_
#define MINIGAME_HPP_

#include <string>

class MiniGame {
 public:
  virtual ~MiniGame() = default;

  //true, если игрок выиграл
  virtual bool Play() = 0;
  //название для Echo и меню
  virtual std::string Name() const = 0;
};

#endif  // MINIGAME_HPP_
