#include "include/GameEngine.hpp"
#include "include/Console.hpp"
#include <iostream>

int main() {
    Console::init();
    GameEngine engine;
    engine.run();
    return 0;
}