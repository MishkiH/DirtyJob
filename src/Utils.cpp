#include "Utils.hpp"

namespace Utils {

void SleepFor(const int& ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void ClearScreen() {
  // Получаем дескриптор консоли
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  // Получаем информацию о консольном буфере
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
    std::cerr << "Ошибка при получении информации о консольном буфере."
              << std::endl;
    return;
  }

  // Размеры экрана
  int cols = csbi.dwSize.X;
  int rows = csbi.dwSize.Y;

  // Создаём пустой буфер для очистки экрана
  COORD bufferSize = {static_cast<SHORT>(cols), static_cast<SHORT>(rows)};
  COORD bufferCoord = {0, 0};
  SMALL_RECT writeRegion = {0, 0, static_cast<SHORT>(cols - 1),
                            static_cast<SHORT>(rows - 1)};

  // Буфер с пробелами (для очистки экрана)
  std::vector<CHAR_INFO> buffer(cols * rows);
  for (auto& charInfo : buffer) {
    charInfo.Char.AsciiChar = ' ';  // Пробел
    charInfo.Attributes = 0x00;     // Черный фон (без текста)
  }

  // Очищаем экран
  WriteConsoleOutput(hConsole, buffer.data(), bufferSize, bufferCoord,
                     &writeRegion);
}

void PrintSlowlyByChar(const std::string& text) {
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
  return text;
}

}  // namespace Utils
