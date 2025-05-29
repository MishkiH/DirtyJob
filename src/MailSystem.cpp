#include "../include/MailSystem.hpp"
#include "../include/Console.hpp"
#include "../include/components/HasInventory.hpp"
#include "../include/components/HasStats.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

MailSystem::MailSystem() {
}

void MailSystem::loadMails() {
    generateDefaultMails();
}

void MailSystem::generateDefaultMails() {
    mails.emplace_back(1, "Срочная замена аудиодорожки", 
        "Приветствую!\n\n"
        "Мой давний знакомый рекомендовал вас как профессионала, который умеет решать задачи без лишних вопросов.\n\n"
        "На видеохостинге нашего партнёра залита запись концерта группы DaiJadzu. Чёртов дрон заснял оплошность басиста – "
        "неудачные несколько аккордов, испортившие всё выступление. Для мировой публики подобная оплошность недопустима.\n\n"
        "Нужно как можно скорее заменить оригинальный звук до старта трансляции. Мы уже подготовили исправленную аудиодорожку.\n\n"
        "Предоплату на указанный криптокошелёк выслали. Времени мало, но уверены, что вы справитесь.\n\n"
        "— Менеджер DaiJadzu Records",
        "anonymous@protonmail.com", 3);
    
    mails.emplace_back(2, "Коррекция финансовых данных",
        "Добрый день.\n\n"
        "Обращаюсь к вам по рекомендации. У нас возникла деликатная ситуация с отчетностью.\n\n"
        "Нужно внести небольшие правки в базу данных налоговой службы. Ничего критичного - просто скорректировать "
        "несколько цифр в декларации нашей компании. Система защищена, но для профессионала это не проблема.\n\n"
        "Оплата достойная. Конфиденциальность гарантирую.\n\n"
        "Жду ответа в течение 24 часов.\n\n"
        "— Финансовый консультант",
        "consultant.fin@temp-mail.org", 5);
        
    mails.emplace_back(3, "Удаление компрометирующих записей",
        "Здравствуйте.\n\n"
        "Я политик, и недавно в сети появились записи, которые могут серьёзно повредить моей карьере. "
        "Технически это правда, но контекст представлен неверно.\n\n"
        "Нужно удалить эти записи из всех доступных источников: социальные сети, новостные порталы, "
        "архивы сайтов. Также необходимо заменить информацию в некоторых базах данных.\n\n"
        "Готов заплатить любую сумму. Это вопрос моего политического будущего.\n\n"
        "P.S. Работа должна быть выполнена незаметно.\n\n"
        "— Заинтересованное лицо",
        "political.crisis@secure-mail.net", 8);
}

void MailSystem::showInterface() {
    std::cout << Console::colorize("╔═══════════════════════════════════════════════════════════════╗\n", Console::CYAN);
    std::cout << Console::colorize("║                        ПОЧТОВЫЙ КЛИЕНТ                       ║\n", Console::CYAN);
    std::cout << Console::colorize("╚═══════════════════════════════════════════════════════════════╝\n", Console::CYAN);
    
    displayMailList();
    
    std::cout << "\nДоступные команды:\n";
    std::cout << Console::colorize("/read {номер}", Console::GREEN) << " - прочитать письмо\n";
    std::cout << Console::colorize("/take {номер}", Console::GREEN) << " - принять заказ\n";
    std::cout << Console::colorize("/switch main", Console::GREEN) << " - вернуться в главное меню\n";
}

void MailSystem::displayMailList() const {
    std::cout << "\n" << Console::colorize("📧 ВХОДЯЩИЕ СООБЩЕНИЯ:", Console::YELLOW) << "\n\n";
    
    for (const auto& mail : mails) {
        std::string status = mail.isRead ? "📖" : "📩";
        std::cout << status << Console::colorize(" Письмо №" + std::to_string(mail.id), Console::WHITE);
        std::cout << Console::colorize(". Тема: ", Console::GRAY) << Console::colorize(mail.subject, Console::YELLOW) << "\n";
        std::cout << Console::colorize("   Награда: ", Console::GREEN) << mail.reward << " ETH\n\n";
    }
}

void MailSystem::displayMail(int id) const {
    auto it = std::find_if(mails.begin(), mails.end(),
        [id](const Mail& mail) { return mail.id == id; });
    
    if (it == mails.end()) {
        std::cout << Console::colorize("Письмо не найдено.\n", Console::RED);
        return;
    }
    
    const auto& mail = *it;
    
    std::cout << Console::colorize("═══════════════════════════════════════════════════════════════\n", Console::BLUE);
    std::cout << Console::colorize("От: ", Console::GRAY) << mail.sender << "\n";
    std::cout << Console::colorize("Тема: ", Console::GRAY) << Console::colorize(mail.subject, Console::YELLOW) << "\n";
    std::cout << Console::colorize("Награда: ", Console::GRAY) << Console::colorize(std::to_string(mail.reward) + " ETH", Console::GREEN) << "\n";
    std::cout << Console::colorize("═══════════════════════════════════════════════════════════════\n", Console::BLUE);
    std::cout << "\n" << mail.content << "\n\n";
}

bool MailSystem::processCommand(const std::string& command, Entity& player) {
    if (command.substr(0, 6) == "/read ") {
        int id = std::stoi(command.substr(6));
        displayMail(id);
        
        // Отмечаем как прочитанное
        auto it = std::find_if(mails.begin(), mails.end(),
            [id](Mail& mail) { return mail.id == id; });
        if (it != mails.end()) {
            it->isRead = true;
        }
        return true;
    }
    
    if (command.substr(0, 6) == "/take ") {
        int id = std::stoi(command.substr(6));
        return takeMission(id, player);
    }
    
    return false;
}

bool MailSystem::takeMission(int id, Entity& player) {
    auto it = std::find_if(mails.begin(), mails.end(),
        [id](const Mail& mail) { return mail.id == id; });
    
    if (it == mails.end()) {
        std::cout << Console::colorize("Задание не найдено.\n", Console::RED);
        return false;
    }
    
    const auto& mail = *it;
    
    std::cout << Console::colorize("═══ ПРИНЯТИЕ ЗАДАНИЯ ═══\n", Console::YELLOW);
    std::cout << "Задание: " << Console::colorize(mail.subject, Console::CYAN) << "\n";
    std::cout << "Награда: " << Console::colorize(std::to_string(mail.reward) + " ETH", Console::GREEN) << "\n\n";
    
    std::cout << Console::colorize("🔧 Запуск системы взлома...\n", Console::BLUE);
    std::cout << Console::colorize("⚡ Инициализация BruteForcer v2.13...\n", Console::YELLOW);
    std::cout << Console::colorize("🔑 Подбор ключей доступа...\n", Console::GREEN);
    
    bool success = true; // В демо всегда успех
    
    if (success) {
        std::cout << Console::colorize("✅ ВЗЛОМ УСПЕШЕН!\n", Console::GREEN);
        std::cout << Console::colorize("📊 Данные изменены\n", Console::CYAN);
        std::cout << Console::colorize("🚪 Выход из системы...\n", Console::BLUE);
        
        //награждаем игрока
        auto inventory = player.getComponent<HasInventory>();
        if (inventory) {
            inventory->addEthereum(mail.reward);
            std::cout << Console::colorize("💰 Получено " + std::to_string(mail.reward) + " ETH\n", Console::GREEN);
        }
        
        //влияние на характеристики в зависимости от задания
        auto stats = player.getComponent<HasStats>();
        if (stats) {
            if (id == 1) { // замена аудиодорожки - относительно безобидно
                stats->modifyPragmatism(5);
                std::cout << Console::colorize("📈 Прагматизм +5\n", Console::BLUE);
            } else if (id == 2) { // финансовые махинации
                stats->modifyPragmatism(10);
                stats->modifyCriticalThinking(-3);
                std::cout << Console::colorize("📈 Прагматизм +10, Критическое мышление -3\n", Console::BLUE);
            } else if (id == 3) { //удаление компромата
                stats->modifyPragmatism(15);
                stats->modifyRebelSpirit(8);
                stats->modifyCriticalThinking(-5);
                std::cout << Console::colorize("📈 Прагматизм +15, Бунтарский дух +8, Критическое мышление -5\n", Console::BLUE);
            }
        }
        
        // чистим выполненное задание
        mails.erase(it);
        
    } else {
        std::cout << Console::colorize("❌ ВЗЛОМ ПРОВАЛЕН!\n", Console::RED);
        std::cout << Console::colorize("🚨 Обнаружена атака\n", Console::RED);
        
        auto stats = player.getComponent<HasStats>();
        if (stats) {
            stats->modifyHealth(-20);
            std::cout << Console::colorize("💔 Здоровье -20\n", Console::RED);
        }
    }
    
    return true;
}