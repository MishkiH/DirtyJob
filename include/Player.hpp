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
    AddComponent(std::make_unique<MoralComponent>(0));
    AddComponent(std::make_unique<HealthComponent>(3));
  }
  InventoryComponent* Inventory() { return GetComponent<InventoryComponent>(); }
  MoralComponent* Moral() { return GetComponent<MoralComponent>(); }
  HealthComponent* Health() { return GetComponent<HealthComponent>(); }
  void ActivateCoffee() { coffee_boost_ = true; }
  void ResetCoffee() { coffee_boost_ = false; }
  bool HasCoffee() const { return coffee_boost_; }
private:
    bool coffee_boost_ = false;
};

#endif  // PLAYER_HPP_
