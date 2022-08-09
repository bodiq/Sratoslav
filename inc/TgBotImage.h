#pragma once

#include <ctime>
#include <string>
#include "CImg.h"
#include <fstream>
#include <tgbot/tgbot.h>
#include "Phrases.h"


bool check_is_num(std::string &text, int &num)
{
    if(text.size() > 1)
    {
        num = std::atoi(text.c_str());
        return true;
    }
    if(text.size() == 1)
    {
        if(isdigit(text[0]))
        {
            num = std::atoi(text.c_str());
            return true;
        }
    }
    return false;
}

void delete_phrase(Phrases &phrases, TgBot::Bot &bot, bool &state)
{
    int num;
    bot.getEvents().onAnyMessage([&, state] (TgBot::Message::Ptr message) mutable
     {
         if(message->text == "DeletePhrase")
         {
             bot.getApi().sendMessage(message->chat->id, "What number of this list you want to delete: ");
         }
         else
         {
             if(check_is_num(message->text, num) && state)
             {
                 phrases.deletePhrase(num, bot, message);
                 state = false;
             }
             else if(!check_is_num(message->text, num) && state)
             {
                 bot.getApi().sendMessage(message->chat->id, "What number of this list you want to delete: ");
             }
         }
     });
}

bool check_is_not_command(const std::vector<std::string> &commands, const std::string &text)
{
    for(size_t i = 0; i < commands.size(); i++)
    {
        if(commands[i] == text)
        {
            return false;
        }
    }
    return true;
}

void add_phrs(TgBot::Bot &bot, const std::vector<std::string> &commands, Phrases &phrases, bool &check)
{
    bot.getEvents().onAnyMessage([&, check] (TgBot::Message::Ptr message) mutable
     {
         if(check_is_not_command(commands, message->text) && check)
         {
             phrases.addPhrase(message->text, bot, message);
             check = false;
         }
     });
}

int getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

const unsigned char *color(const std::string &col)
{
    const unsigned char cyan[] = { 0, 255, 255 };
    const unsigned char black[] = { 0, 0, 0 };
    const unsigned char yellow[] = { 255, 255, 0 };
    const unsigned char red[] = { 255, 0, 0 };
    const unsigned char green[] = { 0, 255, 0 };
    const unsigned char orange[] = { 255, 165, 0 };
    const unsigned char blue[] = { 0,0,255 };

    if(col == "black")
    {
        return black;
    }
    else if(col == "cyan")
    {
        return cyan;
    }
    else if(col == "yellow")
    {
        return yellow;
    }
    else if(col == "red")
    {
        return red;
    }
    else if(col == "green")
    {
        return green;
    }
    else if(col == "orange")
    {
        return orange;
    }
    else if(col == "blue")
    {
        return blue;
    }
    return black;
}

void draw_text(cimg_library::CImg< unsigned char > &image, const std::string &text, const std::string &col, std::string &save_to)
{
    int tile_size = image.width();
    int row_col_count = image.height();

    std::cout << col << std::endl;
    const unsigned char *text_color = color(col);

    image.draw_text((tile_size / 2) - (tile_size / 100 * 10), (row_col_count / 2) + (row_col_count / 100 * 40), text.c_str(), text_color, 1, 1, 25);
    image.save_jpeg(save_to.c_str());
}

void doMagic(Phrases &phr, std::string &text, std::string &color, bool &text_check, bool &color_check, bool &photo_check, bool &photo_only, TgBot::Bot &bot, const std::vector<std::string> &texts, const std::string &photoMimeType, bool &all_in_one)
{
    bot.getEvents().onAnyMessage([&, text_check, color_check, photo_check, photo_only, all_in_one] (TgBot::Message::Ptr message) mutable
     {
         if(all_in_one)
         {
             all_in_one = false;
             int num = std::atoi(message->text.c_str());
             text = phr[num - 1];
             bot.getApi().sendMessage(message->chat->id,"What color do you want? [black, blue, cyan, yellow, red, orange, green]");
             color_check = true;
         }
         if (text_check)
         {
             if (!message->text.empty() && message->text != "/PhotoAndText")
             {
                 text_check = false;
                 text = message->text;
                 bot.getApi().sendMessage(message->chat->id,"What color do you want? [black, blue, cyan, yellow, red, orange, green]");
                 color_check = true;
             }
             else
             {
                 bot.getApi().sendMessage(message->chat->id, "Send me a text: ");
             }
         }

         if (color_check)
         {
             if (message->text == "black" || message->text == "blue" || message->text == "yellow" || message->text == "cyan" || message->text == "red" || message->text == "orange" || message->text == "green")
             {
                 color_check = false;
                 color = message->text;
                 photo_check = true;
                 bot.getApi().sendMessage(message->chat->id, "Send me a Photo: ");
             }
         }
         if (photo_check)
         {
             if (!message->photo.empty())
             {
                 std::string Photo_id = message->photo[2]->fileId;
                 std::string toDownn = bot.getApi().getFile(Photo_id)->filePath;
                 std::string a = bot.getApi().downloadFile(toDownn);
                 std::ofstream myfile;
                 std::string filePath = "/Users/bodya/Downloads/Test/images/" + message->photo[2]->fileId + ".jpg";
                 std::string save_to = "/Users/bodya/Downloads/Test/images/" + message->photo[2]->fileId + "1.jpg";
                 myfile.open(filePath);
                 myfile << a;
                 cimg_library::CImg<unsigned char> image = cimg_library::CImg<unsigned char>(filePath.c_str());
                 if (photo_only)
                 {
                     draw_text(image, texts[getRandomNumber(0, texts.size())], color, save_to);
                 }
                 else
                 {
                     draw_text(image, text, color, save_to);
                 }
                 bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile(save_to, photoMimeType));
                 myfile.close();
                 photo_check = false;
                 photo_only = false;
                 std::remove(filePath.c_str());
                 std::remove(save_to.c_str());
             }
         }
     });
}