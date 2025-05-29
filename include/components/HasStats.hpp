#pragma once
#include "../Component.hpp"

class HasStats : public Component {
public:
    HasStats();
    ~HasStats() override = default;
    
    // для характеристик
    int getCriticalThinking() const { return criticalThinking; }
    int getPragmatism() const { return pragmatism; }
    int getRebelSpirit() const { return rebelSpirit; }
    int getHealth() const { return health; }
    
    //для характеристик
    void setCriticalThinking(int value) { criticalThinking = clamp(value); }
    void setPragmatism(int value) { pragmatism = clamp(value); }
    void setRebelSpirit(int value) { rebelSpirit = clamp(value); }
    void setHealth(int value) { health = clamp(value, 0, 100); }
    
    void modifyCriticalThinking(int delta) { criticalThinking = clamp(criticalThinking + delta); }
    void modifyPragmatism(int delta) { pragmatism = clamp(pragmatism + delta); }
    void modifyRebelSpirit(int delta) { rebelSpirit = clamp(rebelSpirit + delta); }
    void modifyHealth(int delta) { health = clamp(health + delta, 0, 100); }
    
    void displayStats() const;
    void restoreHealth(int amount) { modifyHealth(amount); }
    
private:
    int criticalThinking; // 0-100
    int pragmatism;     // 0-100
    int rebelSpirit;  // 0-100
    int health; // 0-100
    
    int clamp(int value, int min = 0, int max = 100) const;
};