#include <string>
#include "CImg.h"
#include <vector>
#include <iostream>

#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;
using namespace cimg_library;
using namespace std;


void draw_text(CImg< unsigned char > &image, const char *text)
{
    static int tile_size = image.width();
    static int row_col_count = image.height();

    unsigned char cyan[] = { 0, 255, 255 };
    unsigned char black[] = { 0, 0, 0 };
    unsigned char yellow[] = { 255, 255, 0 };
    unsigned char red[] = { 255, 0, 0 };
    unsigned char green[] = { 0, 255, 0 };
    unsigned char orange[] = { 255, 165, 0 };
    unsigned char blue[] = { 0,0,255 };

    image.draw_text((row_col_count / 2) - 50, 450, text, black, 1, 1, 25);
    image.save_jpeg("/Users/bodya/Downloads/Test/images/ex1.png");
}

std::vector<string> bot_commands = {"start", "test"};

int main()
{
    TgBot::Bot bot("5415417859:AAEI59uufbkUzTjWIUi5qLHB-uOKQ42UX-0");


    const std::string photoFilePath = "/Users/bodya/Downloads/Test/images/ex1.png";
    const std::string photoMimeType = "image/png";

    bool test_text_state = false;
    bool photo_check = false;
    bool photo_upload = false;
    std::string text;

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });
    bot.getEvents().onCommand("Photo", [&](Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "Enter text");
    });
    bot.getEvents().onCommand("test", [&](Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "Enter text");
        test_text_state = true;
    });

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message)
    {
        if (test_text_state)
        {
            text = message->text.c_str();
            std::cout << text << std::endl;
            test_text_state = false;
            photo_check = true;
        }
        if(photo_check)
        {
            bot.getApi().sendMessage(message->chat->id, "Send Photo");
            photo_upload = true;
            photo_check = false;
        }
        if(photo_upload)
        {
            sleep(6);
            std::cout << "First" << std::endl;
            std::string info = bot.getApi().getFile(message->chat->photo->bigFileId)->filePath;
            std::cout << "First" << std::endl;
            std::string download = bot.getApi().downloadFile(info);

            std::cout << download << std::endl;
            CImg< unsigned char > image = CImg< unsigned char >(download.c_str());
            draw_text(image, text.c_str());

            bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath, photoMimeType));
        }
    });
    /*bot.getEvents().onCommand("photo", [&bot, &photoFilePath, &photoMimeType](Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "Enter text");
        test_text_state = true;
        bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath, photoMimeType));
    });*/

    signal(SIGINT, [](int s)
    {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}