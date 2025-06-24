#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Entity.hpp"
#include "InventoryComponent.hpp"
#include "MoralComponent.hpp"
#include "HealthComponent.hpp"

class Player : public Entity {
 public:
  Player() {
    AddComponent(std::make_unique<InventoryComponent>());
    AddComponent(std::make_unique<MoralComponent>(0));    // Начальное значение из json
    AddComponent(std::make_unique<HealthComponent>(3));   // Можно читать из json
  }

  InventoryComponent* Inventory() { return GetComponent<InventoryComponent>(); }
  MoralComponent* Moral() { return GetComponent<MoralComponent>(); }
  HealthComponent* Health() { return GetComponent<HealthComponent>(); }
};

#endif  // PLAYER_HPP_
