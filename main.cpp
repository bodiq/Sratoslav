#include <string>
#include "CImg.h"
#include <vector>
#include <iostream>
#include <fstream>

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
    image.save_jpeg("/Users/bodya/Downloads/Test/images/ex1.jpg");
}

std::vector<string> bot_commands = {"start", "test"};

int main()
{
    TgBot::Bot bot("5415417859:AAEI59uufbkUzTjWIUi5qLHB-uOKQ42UX-0");


    const std::string photoFilePath = "/Users/bodya/Downloads/Test/images/ex1.jpg";
    const std::string photoMimeType = "image/jpg";

    bool test_text_state = false;
    bool photo_check = false;
    bool photo_upload = false;
    std::string text;

    bot.getEvents().onCommand("Photo", [&](Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "Send me a text: ");
        photo_upload = true;
    });

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message)
    {
        if(photo_upload)
        {
            text = message->text;
            photo_check = true;
            photo_upload = false;
            bot.getApi().sendMessage(message->chat->id, "Send me a photo: ");
        }
        if(photo_check)
        {
            if(!message->photo.empty())
            {
                std::string Photo_id = message->photo[2]->fileId;
                std::string toDownn = bot.getApi().getFile(Photo_id)->filePath;
                std::string a = bot.getApi().downloadFile(toDownn);
                ofstream myfile;
                myfile.open("/Users/bodya/Downloads/Test/images/ex.jpg");
                myfile << a;
                CImg< unsigned char > image = CImg< unsigned char >("/Users/bodya/Downloads/Test/images/ex.jpg");
                draw_text(image, text.c_str());
                bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath, photoMimeType));
                myfile.close();
                photo_check = false;
                std::remove("/Users/bodya/Downloads/Test/images/ex.jpg");
                std::remove("/Users/bodya/Downloads/Test/images/ex1.jpg");
            }
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