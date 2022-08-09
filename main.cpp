#include "inc/TgBotImage.h"
#include "Pool/ThreadPool/ThreadPool.h"
#include "Pool/Async/Async.h"
#include "DataBase/DBLite.h"

int main(int argc, char** argv)
{
    ExecuteInterface *ex;

    if(std::string(argv[1]) == "async")
    {
        ex = new Async;
    }
    else if(std::string(argv[1]) == "thread")
    {
        ex = new ThreadPool;
    }

    srand(time(nullptr));

    TgBot::Bot bot("TOKEN");
    Phrases phrases;
    DBLite dbLite;

    int user_id = rand() % 100;

    dbLite.createUserTable();
    dbLite.createPhraseTable();


    const std::string photoMimeType = "image/jpg";
    const std::vector<std::string> texts = {"What a guy!", "Persik a ne hlop", "Dont look at me like a stupid","You are mad(;"};
    const std::vector<std::string> command = {"/Start", "/PhotoOnly", "/PhotoAndText", "/AddPhrase", "/ShowPhrase", "/DeletePhrase", "/Quit"};

    std::string all_commands;

    for(size_t i = 0; i < command.size(); i++)
    {
        all_commands = all_commands + command[i] + ", ";
    }

    bool photo_check = false;
    bool text_check = false;
    bool color_check = false;
    bool photo_only_check = false;
    bool all_in_one = false;

    std::atomic<bool> interface{false};  // false for thread true for async

    std::string text;
    std::string color;

    std::condition_variable cov;
    std::mutex mtx;

    bot.getEvents().onCommand("DeletePhrase", [&] (TgBot::Message::Ptr message)
    {
        ex->addJob([&, message] () mutable
        {
              if(phrases.showAll(bot, message))
              {
                  bool state = true;
                  bot.getApi().sendMessage(message->chat->id, "What number of this list you want to delete: ");
                  delete_phrase(phrases, bot, state);
              }
        });
    });

    bot.getEvents().onCommand("ShowPhrase", [&] (TgBot::Message::Ptr message)
    {
        ex->addJob([&, message] () mutable
        {
            phrases.showAll(bot, message);
        });
    });

    bot.getEvents().onCommand("AddPhrase", [&] (TgBot::Message::Ptr message)
    {
        ex->addJob([&, message] () mutable
        {
              bool state = true;
              bot.getApi().sendMessage(message->chat->id, "Enter phrase you want to add");
              add_phrs(bot, command, phrases, state);
        });
    });

    bot.getEvents().onCommand("commands", [&] (TgBot::Message::Ptr message)
    {
        ex->addJob([&, message] () mutable
        {
            bot.getApi().sendMessage(message->chat->id, "Available commands: " + all_commands);
        });
    });

    bot.getEvents().onCommand("Start", [&](TgBot::Message::Ptr message)
    {
        ex->addJob([&, message] () mutable
          {
              const std::string username = message->chat->username.c_str();
              bot.getApi().sendMessage(message->chat->id,"Hi, " + username + "\nTo see all the available commands enter: /commands");
              dbLite.insertData("USERS", user_id, message->chat->username);
          });
    });

    bot.getEvents().onCommand("PhotoOnly", [&](TgBot::Message::Ptr message)
    {
        ex->addJob([&, message] () mutable
        {
              bot.getApi().sendMessage(message->chat->id,"What color do you want? [black, blue, cyan, yellow, red, orange, green]");
              color_check = true;
              photo_only_check = true;
              text_check = false;
              doMagic(phrases, text, color, text_check, color_check, photo_check, photo_only_check, bot, texts, photoMimeType, all_in_one);
        });
    });

    bot.getEvents().onCommand("PhotoAndText", [&](TgBot::Message::Ptr message)
    {
        ex->addJob([&, message] () mutable
        {
              bot.getApi().sendMessage(message->chat->id, "Choose the phrase from list, or add by /AddPhrase");
              if(phrases.showAll(bot, message))
              {
                  all_in_one = true;
                  doMagic(phrases, text, color, text_check, color_check, photo_check, photo_only_check, bot, texts, photoMimeType, all_in_one);
              }
        });
    });

    bot.getEvents().onCommand("Quit", [&](TgBot::Message::Ptr message)
    {
        ex->addJob([&, message] () mutable
        {
              bot.getApi().sendMessage(message->chat->id, "GoodBye");
              dbLite.insertData("PHRASES", user_id, phrases.getPhrases());
              dbLite.closeDB();
        });
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try
    {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(bot);
        while (true)
        {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (std::exception &e)
    {
        printf("error: %s\n", e.what());
    }
    catch (std::string &a)
    {
        std::cerr << a << std::endl;
    }
    return 0;
}
