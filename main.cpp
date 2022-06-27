#include "TgBotImage.h"

int main()
{
    srand(time(NULL));

    TgBot::Bot bot("5415417859:AAEI59uufbkUzTjWIUi5qLHB-uOKQ42UX-0");

    const std::string photoFilePath = "/Users/bodya/Downloads/Test/images/ex1.jpg";
    const std::string photoMimeType = "image/jpg";

    const std::vector<std::string> texts = {"What a guy!", "Persik a ne hlop", "Dont look at me like a stupid", "You are mad(;"};

    bool photo_check = false;
    bool text_check = false;
    bool color_check = false;
    bool photo_only_check = false;

    std::string text;
    std::string color;

    bot.getEvents().onCommand("Start", [&](TgBot::Message::Ptr message)
    {
        const std::string username = message->chat->username.c_str();
        bot.getApi().sendMessage(message->chat->id, "Hi, " + username + "\nChoose what you want to do: [/PhotoOnly; /PhotoAndText]" );
    });
    bot.getEvents().onCommand("PhotoOnly", [&](TgBot::Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "What color do you want? [black, blue, cyan, yellow, red, orange, green]");
        color_check = true;
        photo_only_check = true;
    });
    bot.getEvents().onCommand("PhotoAndText", [&](TgBot::Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "Enter your text: ");
        text_check = true;
    });
    bot.getEvents().onCommand("Quit", [&](TgBot::Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "GoodBye");
    });
    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message)
    {
        if(text_check)
        {
            if(!message->text.empty() && message->text != "/PhotoAndText")
            {
                text = message->text;
                text_check = false;
                color_check = true;
                bot.getApi().sendMessage(message->chat->id, "What color do you want? [black, blue, cyan, yellow, red, orange, green]");
            }
            else
            {
                bot.getApi().sendMessage(message->chat->id, "Send me a text: ");
            }
        }
        if(color_check)
        {
            if(message->text == "black" || message->text == "blue" || message->text == "yellow"
            || message->text == "cyan" || message->text == "red" || message->text == "orange" || message->text == "green")
            {
                color = message->text;
                std::cout << color << std::endl;
                photo_check = true;
                bot.getApi().sendMessage(message->chat->id, "Send me a Photo: ");
                color_check = false;
            }
            else
            {
                bot.getApi().sendMessage(message->chat->id, "What color do you want? [black, blue, cyan, yellow, red, orange, green]");
            }
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
                if(photo_only_check)
                {
                    draw_text(image, texts[getRandomNumber(0, texts.size())], color);
                    photo_only_check = false;
                }
                else
                {
                    draw_text(image, text, color);
                }
                bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile(photoFilePath, photoMimeType));
                myfile.close();
                photo_check = false;
                std::remove("/Users/bodya/Downloads/Test/images/ex.jpg");
                std::remove("/Users/bodya/Downloads/Test/images/ex1.jpg");
            }
            else if(message->photo.empty())
            {
                bot.getApi().sendMessage(message->chat->id, "Send me a photo: ");
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