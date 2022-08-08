#include "Phrases.h"

bool Phrases::showAll(TgBot::Bot &bot, TgBot::Message::Ptr &message)
{
    if(phrases.size() <= 0)
    {
        bot.getApi().sendMessage(message->chat->id, "List of Phrases is empty.");
        return false;
    }
    bot.getApi().sendMessage(message->chat->id, "All what you have added: ");
    for(int i = 0; i < phrases.size(); i++)
    {
        bot.getApi().sendMessage(message->chat->id, std::to_string(i + 1) + ". " + phrases[i]);
    }
    return true;
}

void Phrases::addPhrase(const std::string &to_add, TgBot::Bot &bot, TgBot::Message::Ptr &message)
{
    phrases.push_back(to_add);
    bot.getApi().sendMessage(message->chat->id, to_add + " [Phrase add!]");
}

void Phrases::deletePhrase(int number, TgBot::Bot &bot, TgBot::Message::Ptr &message)
{
    if(phrases.size() <= 0)
    {
        bot.getApi().sendMessage(message->chat->id, "List of Phrases is empty.");
        return;
    }
    if(number - 1 >= phrases.size() || number < 0)
    {
        bot.getApi().sendMessage(message->chat->id, "No such number in list");
        return;
    }
    bot.getApi().sendMessage(message->chat->id, phrases[number - 1] + " [Has been deleted!]");
    phrases.erase(phrases.begin() + number - 1);
}

std::string Phrases::operator[](int index)
{
    if(index < 0 || index >= phrases.size())
    {
        throw (std::string)("Bad");
    }
    return phrases[index];
}