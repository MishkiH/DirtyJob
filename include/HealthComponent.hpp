#ifndef HEALTH_COMPONENT_HPP_
#define HEALTH_COMPONENT_HPP_

#include "Component.hpp"

class HealthComponent : public Component {
 public:
  HealthComponent(int initial_hp = 2) : max_hp_(initial_hp), hp_(initial_hp) {}
  ~HealthComponent() override = default;

  std::string GetID() const override { return "HealthComponent"; }

  int Get() const { return hp_; }
  int Max() const { return max_hp_; }
  void Damage(int dmg = 1) { hp_ -= dmg; }
  void Heal(int amount = 1) {
    (hp_ + amount > max_hp_) ? max_hp_ : hp_ + amount;
    ;
  }
  void UpgradeMax() {
    max_hp_ += 1;
    hp_ = max_hp_;
  }

 private:
  int max_hp_;
  int hp_;
};

#endif  // HEALTH_COMPONENT_HPP_
