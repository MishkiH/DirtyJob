#ifndef MINIGAMES_PROTOCOLSIMON_HPP_
#define MINIGAMES_PROTOCOLSIMON_HPP_

#include "MiniGame.hpp"

class ProtocolSimon : public MiniGame {
 public:
  ProtocolSimon() = default;
  bool Play() override;
  std::string Name() const override { return "Protocol Simon"; }
};

#endif  // MINIGAMES_PROTOCOLSIMON_HPP_
