#ifndef MORAL_COMPONENT_HPP_
#define MORAL_COMPONENT_HPP_

#include "Component.hpp"

class MoralComponent : public Component {
 public:
  MoralComponent(int initial_value = 0) : value_(initial_value) {}
  ~MoralComponent() override = default;

  std::string GetID() const override { return "MoralComponent"; }

  int GetValue() const { return value_; }
  void Add(int diff) { value_ += diff; }

 private:
  int value_;
};

#endif  // MORAL_COMPONENT_HPP_
