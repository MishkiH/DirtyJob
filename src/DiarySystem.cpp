#include "../include/DiarySystem.hpp"
#include "../include/Console.hpp"
#include "../include/components/HasStats.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

DiarySystem::DiarySystem() {
}

void DiarySystem::loadDiary() {
    generateDefaultEntries();
}

void DiarySystem::generateDefaultEntries() {
    entries.emplace_back(1, "Press Start", 
        "Чего стоит правда? Стандартный прайс, который я беру за заказы – 2 монетки ETH. "
        "Этого достаточно, чтобы поменять чью-то жизнь в лучшую или худшую сторону. Мне безразлично. Деньги не пахнут.\n\n"
        "А правда?.. Пхах, её не существует. Есть только информация. А информацию можно изменить. "
        "Я решил начать вести эти записи, чтобы хоть что-то осталось незапятнанным.\n\n"
        "Первый день в новой реальности. Интересно, кем я стану через месяц?",
        "2024-01-15");
        
    entries.emplace_back(2, "Первый заказ",
        "Сегодня выполнил первое задание. Замена аудиодорожки для какой-то группы. "
        "Казалось бы, ерунда - кто заметит пару неточных нот? Но для них это вопрос репутации.\n\n"
        "Смешно, как люди цепляются за иллюзию совершенства. Хотя... разве я не делаю то же самое? "
        "Стираю следы, создаю новую реальность.\n\n"
        "3 ETH за полчаса работы. Неплохо для начала.",
        "2024-01-16");
        
    entries.emplace_back(3, "Размышления о морали",
        "Сегодня получил заказ на изменение финансовых данных. Налоговая служба, декларации... "
        "Клиент говорит, что это 'небольшие правки'. Но я же не дурак.\n\n"
        "Вопрос: где граница? Когда помощь становится соучастием? Когда техническое решение "
        "превращается в преступление?\n\n"
        "Пока что я просто инструмент. Клавиатура не несет ответственности за то, что печатают. "
        "Или несет?",
        "2024-01-17");
}

void DiarySystem::showInterface() {
    std::cout << Console::colorize("╔═══════════════════════════════════════════════════════════════╗\n", Console::CYAN);
    std::cout << Console::colorize("║                           ДНЕВНИК                            ║\n", Console::CYAN);
    std::cout << Console::colorize("╚═══════════════════════════════════════════════════════════════╝\n", Console::CYAN);
    
    displayEntryList();
    
    std::cout << "\nДоступные команды:\n";
    std::cout << Console::colorize("/read {номер}", Console::GREEN) << " - прочитать запись\n";
    std::cout << Console::colorize("/publish {номер}", Console::GREEN) << " - опубликовать запись\n";
    std::cout << Console::colorize("/delete {номер}", Console::GREEN) << " - удалить запись\n";
    std::cout << Console::colorize("/switch main", Console::GREEN) << " - вернуться в главное меню\n";
}

void DiarySystem::displayEntryList() const {
    std::cout << "\n" << Console::colorize("📝 ЗАПИСИ ДНЕВНИКА:", Console::YELLOW) << "\n\n";
    
    for (const auto& entry : entries) {
        std::string status = entry.isPublished ? "📢" : "📋";
        std::cout << status << Console::colorize(" Запись №" + std::to_string(entry.id), Console::WHITE);
        std::cout << Console::colorize(". ", Console::GRAY) << Console::colorize(entry.title, Console::YELLOW) << "\n";
        std::cout << Console::colorize("   Дата: ", Console::GRAY) << entry.date << "\n\n";
    }
}

void DiarySystem::displayEntry(int id) const {
    auto it = std::find_if(entries.begin(), entries.end(),
        [id](const DiaryEntry& entry) { return entry.id == id; });
    
    if (it == entries.end()) {
        std::cout << Console::colorize("Запись не найдена.\n", Console::RED);
        return;
    }
    
    const auto& entry = *it;
    
    std::cout << Console::colorize("═══════════════════════════════════════════════════════════════\n", Console::BLUE);
    std::cout << Console::colorize("Заголовок: ", Console::GRAY) << Console::colorize(entry.title, Console::YELLOW) << "\n";
    std::cout << Console::colorize("Дата: ", Console::GRAY) << entry.date << "\n";
    std::cout << Console::colorize("Статус: ", Console::GRAY) << (entry.isPublished ? Console::colorize("Опубликовано", Console::GREEN) : Console::colorize("Черновик", Console::YELLOW)) << "\n";
    std::cout << Console::colorize("═══════════════════════════════════════════════════════════════\n", Console::BLUE);
    std::cout << "\n" << entry.content << "\n\n";
}

bool DiarySystem::processCommand(const std::string& command, Entity& player) {
    if (command.substr(0, 6) == "/read ") {
        int id = std::stoi(command.substr(6));
        displayEntry(id);
        return true;
    }
    
    if (command.substr(0, 9) == "/publish ") {
        int id = std::stoi(command.substr(9));
        return publishEntry(id, player);
    }
    
    if (command.substr(0, 8) == "/delete ") {
        int id = std::stoi(command.substr(8));
        
        auto it = std::find_if(entries.begin(), entries.end(),
            [id](const DiaryEntry& entry) { return entry.id == id; });
        
        if (it != entries.end()) {
            std::cout << Console::colorize("Запись '" + it->title + "' удалена.\n", Console::RED);
            
            // Влияние удаления записи на характеристики
            auto stats = player.getComponent<HasStats>();
            if (stats) {
                stats->modifyPragmatism(3);
                stats->modifyCriticalThinking(-2);
                std::cout << Console::colorize("📈 Прагматизм +3, Критическое мышление -2\n", Console::BLUE);
            }
            
            entries.erase(it);
        } else {
            std::cout << Console::colorize("Запись не найдена.\n", Console::RED);
        }
        return true;
    }
    
    return false;
}

bool DiarySystem::publishEntry(int id, Entity& player) {
    auto it = std::find_if(entries.begin(), entries.end(),
        [id](DiaryEntry& entry) { return entry.id == id; });
    
    if (it == entries.end()) {
        std::cout << Console::colorize("Запись не найдена.\n", Console::RED);
        return false;
    }
    
    if (it->isPublished) {
        std::cout << Console::colorize("Запись уже опубликована.\n", Console::YELLOW);
        return true;
    }
    
    it->isPublished = true;
    
    std::cout << Console::colorize("═══ ПУБЛИКАЦИЯ ЗАПИСИ ═══\n", Console::YELLOW);
    std::cout << "Запись: " << Console::colorize(it->title, Console::CYAN) << "\n";
    std::cout << Console::colorize("✅ Запись опубликована в сети\n", Console::GREEN);
    
    auto stats = player.getComponent<HasStats>();
    if (stats) {
        if (id == 1) { // "Press Start" - циничная запись
            stats->modifyPragmatism(8);
            stats->modifyRebelSpirit(5);
            std::cout << Console::colorize("📈 Прагматизм +8, Бунтарский дух +5\n", Console::BLUE);
        } else if (id == 2) { // - размышляем о работе
            stats->modifyPragmatism(3);
            stats->modifyCriticalThinking(2);
            std::cout << Console::colorize("📈 Прагматизм +3, Критическое мышление +2\n", Console::BLUE);
        } else if (id == 3) { // - вопросы этики
            stats->modifyCriticalThinking(10);
            stats->modifyPragmatism(-5);
            stats->modifyRebelSpirit(-3);
            std::cout << Console::colorize("📈 Критическое мышление +10, Прагматизм -5, Бунтарский дух -3\n", Console::BLUE);
        }
    }
    return true;
}

void DiarySystem::addEntry(const DiaryEntry& entry) {
    entries.push_back(entry);
}