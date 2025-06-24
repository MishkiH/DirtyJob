#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <string>

class Component {
 public:
  virtual ~Component() = default;

  // каждый компонент должен иметь уникальный строковый id для регистрации.
  virtual std::string GetID() const = 0;
};

#endif  // COMPONENT_HPP_
