#include "Utils.hpp"

namespace Utils {

void SleepFor(const int& ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Utils::ClearScreen() {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD coordScreen = {0, 0};
  DWORD charsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD consoleSize;

  if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
  consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

  FillConsoleOutputCharacter(hConsole, ' ', consoleSize, coordScreen,
                             &charsWritten);
  FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize,
                             coordScreen, &charsWritten);
  SetConsoleCursorPosition(hConsole, coordScreen);
}

void PrintSlowlyByChar(const std::string& text) {
  Utils::SleepFor(2000);
  bool skip = false;
  for (char ch : text) {
    if (!skip && _kbhit()) {  // если нажата клавиша
      char c = _getch();
      if (c == '\r' || c == '\n') {  // если это Enter
        skip = true;
      }
    }
    std::cout << ch << std::flush;
    if (!skip) {
      if (ch == '\n')
        SleepFor(120);
      else if (ch == ' ')
        SleepFor(30);
      else
        SleepFor(15);
    }
  }
  std::cout << '\n';
}

std::string Color(const std::string& text, const std::string& color) {
  if (color == "red") return "\033[31m" + text + "\033[0m";
  if (color == "green") return "\033[32m" + text + "\033[0m";
  if (color == "yellow") return "\033[33m" + text + "\033[0m";
  if (color == "blue") return "\033[34m" + text + "\033[0m";
  if (color == "magenta") return "\033[35m" + text + "\033[0m";
  if (color == "cyan") return "\033[36m" + text + "\033[0m";
  if (color == "black") return "\033[90m" + text + "\033[0m";
  return text;
}

void StartMenu() {
  Utils::ClearScreen();

  std::cout << Utils::Color(R"(
 ██████╗ ██╗██████╗ ████████╗██╗   ██╗         ██╗ ██████╗ ██████╗ 
 ██╔══██╗██║██╔══██╗╚══██╔══╝╚██╗ ██╔╝         ██║██╔═══██╗██╔══██╗
 ██║  ██║██║██████╔╝   ██║    ╚████╔╝          ██║██║   ██║██████╔╝
 ██║  ██║██║██╔══██╗   ██║     ╚██╔╝      ██   ██║██║   ██║██╔══██╗
 ██████╔╝██║██║  ██║   ██║      ██║       ╚█████╔╝╚██████╔╝██████╔╝
 ╚═════╝ ╚═╝╚═╝  ╚═╝   ╚═╝      ╚═╝        ╚════╝  ╚═════╝ ╚═════╝ 
)",
                            "cyan");

  std::cout << "\n\n";
  std::cout << Utils::Color(
      "                    Нажмите Enter, чтобы продолжить...", "cyan");

  std::cin.get();
}

}  // namespace Utils
