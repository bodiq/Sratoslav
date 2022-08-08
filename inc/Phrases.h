#pragma once

#include <tgbot/tgbot.h>

class Phrases
{
public:
    Phrases() = default;
    bool showAll(TgBot::Bot &bot, TgBot::Message::Ptr &message);
    void addPhrase(const std::string &to_add, TgBot::Bot &bot, TgBot::Message::Ptr &message);
    void deletePhrase(int number, TgBot::Bot &bot, TgBot::Message::Ptr &message);

    std::string operator[](int index);
private:
    std::vector<std::string> phrases;
};


