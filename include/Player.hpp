#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Entity.hpp"
#include "HealthComponent.hpp"
#include "InventoryComponent.hpp"
#include "MoralComponent.hpp"

class Player : public Entity {
 public:
  Player() {
    AddComponent(std::make_unique<InventoryComponent>());
    AddComponent(
        std::make_unique<MoralComponent>(0));  // начальное значение из json
    AddComponent(std::make_unique<HealthComponent>(2));  // читать из json
  }

  InventoryComponent* Inventory() { return GetComponent<InventoryComponent>(); }
  MoralComponent* Moral() { return GetComponent<MoralComponent>(); }
  HealthComponent* Health() { return GetComponent<HealthComponent>(); }
};

#endif  // PLAYER_HPP_
