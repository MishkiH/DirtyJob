#include "GameManager.hpp"
#include "windows.h"

int main() {
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);

  GameManager game;
  game.Run();
  return 0;
}
