#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <typeindex>
#include "Component.hpp"

class Entity {
 public:
  Entity() = default;
  ~Entity() = default;

  // Добавить компонент к сущности (владеем через unique_ptr)
  template <typename T>
  void AddComponent(std::unique_ptr<T> component) {
    components_[std::type_index(typeid(T))] = std::move(component);
  }

  // Получить компонент, если он есть (nullptr если нет)
  template <typename T>
  T* GetComponent() {
    auto it = components_.find(std::type_index(typeid(T)));
    if (it != components_.end()) {
      return dynamic_cast<T*>(it->second.get());
    }
    return nullptr;
  }

  // Удалить компонент
  template <typename T>
  void RemoveComponent() {
    components_.erase(std::type_index(typeid(T)));
  }

 private:
  std::map<std::type_index, std::unique_ptr<Component>> components_;
};

#endif  // ENTITY_HPP_
