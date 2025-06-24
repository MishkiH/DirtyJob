#ifndef MINIGAMES_BRUTEFORCER_HPP_
#define MINIGAMES_BRUTEFORCER_HPP_

#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include "MiniGame.hpp"

class BruteForcer : public MiniGame {
 public:
  BruteForcer() = default;
  bool Play() override;
  std::string Name() const override { return "BruteForcer v2.13"; }
};

#endif  // MINIGAMES_BRUTEFORCER_HPP_
