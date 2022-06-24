#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include "CImg.h"
#include <chrono>
#include <iostream>

/*#include <tgbot/tgbot.h>*/

using namespace std;
/*using namespace TgBot;*/
using namespace cimg_library;
using namespace std;


void draw_text(const char *filename, const char *text)
{
    CImg<unsigned char> image (filename);

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
    image.save_jpeg("example1.jpg");
}

int main()
{
    char *filename = new char[12];
    std::strcpy(filename, "example.jpg");

    int len = strlen(filename);

    char *filename1 = new char[len + 2];

    std::strcpy(filename1, filename);
    std::strcpy(filename1 + (len - 5), "1");

    draw_text(filename, "Привіт");

    char arr[] = "Привіт";
    std::cout << arr << std::endl;

    delete []filename1;
    delete []filename;
    return 0;
}