#pragma once
#include "Component.hpp"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <string>

class Entity {
public:
    explicit Entity(const std::string& name = "Unknown");
    ~Entity() = default;
    
    //добавление компонента
    template<typename T>
    void addComponent(std::unique_ptr<T> component) {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        components[std::type_index(typeid(T))] = std::move(component);
    }
    
    //получение компонента
    template<typename T>
    T* getComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }
    
    //проверка наличия компонента
    template<typename T>
    bool hasComponent() const {
        return components.find(std::type_index(typeid(T))) != components.end();
    }
    
    //удаление компонента
    template<typename T>
    void removeComponent() {
        components.erase(std::type_index(typeid(T)));
    }
    
    const std::string& getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }
    
private:
    std::string name;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};