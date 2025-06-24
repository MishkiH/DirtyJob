#ifndef MINIGAMES_PROTOCOLSIMON_HPP_
#define MINIGAMES_PROTOCOLSIMON_HPP_

#include <cctype>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "MiniGame.hpp"

class ProtocolSimon : public MiniGame {
 public:
  ProtocolSimon() = default;
  bool Play() override;
  std::string Name() const override { return "Protocol Simon"; }
};

#endif  // MINIGAMES_PROTOCOLSIMON_HPP_
