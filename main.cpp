#include "TgBotImage.h"

int main()
{
    TgBot::Bot bot("5415417859:AAEI59uufbkUzTjWIUi5qLHB-uOKQ42UX-0");


    const std::string photoFilePath = "/Users/bodya/Downloads/Test/images/ex1.jpg";
    const std::string photoMimeType = "image/jpg";

    bool photo_check = false;
    bool photo_upload = false;
    std::string text;

    bot.getEvents().onCommand("Photo", [&](TgBot::Message::Ptr message)
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
                std::ofstream myfile;
                myfile.open("/Users/bodya/Downloads/Test/images/ex.jpg");
                myfile << a;
                cimg_library::CImg< unsigned char > image = cimg_library::CImg< unsigned char >("/Users/bodya/Downloads/Test/images/ex.jpg");
                draw_text(image, text.c_str());
                bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile(photoFilePath, photoMimeType));
                myfile.close();
                photo_check = false;
                std::remove("/Users/bodya/Downloads/Test/images/ex.jpg");
                std::remove("/Users/bodya/Downloads/Test/images/ex1.jpg");
            }
        }
    });

    signal(SIGINT, [](int s)
    {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}