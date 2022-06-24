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

/* Generate random numbers in an inclusive range. */
int random(int min, int max)
{
    static bool first = true;
    if (first)
    {
        srand(time(NULL));
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

void draw_text(const char *filename, const char *text)
{
    auto t1 = std::chrono::high_resolution_clock::now();


    // Create 2048x2048px image.
    CImg<unsigned char> image (filename);


    static int tile_size = image.width();
    static int row_col_count = image.height();

    // Make some colours.
    unsigned char cyan[] = { 0, 255, 255 };
    unsigned char black[] = { 0, 0, 0 };
    unsigned char yellow[] = { 255, 255, 0 };
    unsigned char red[] = { 255, 0, 0 };
    unsigned char green[] = { 0, 255, 0 };
    unsigned char orange[] = { 255, 165, 0 };
    unsigned char blue[] = { 0,0,255 };

    unsigned char colors [] = { // This is terrible, but I don't now C++ very well.
            cyan[0], cyan[1], cyan[2],
            yellow[0], yellow[1], yellow[2],
            red[0], red[1], red[2],
            green[0], green[1], green[2],
            orange[0], orange[1], orange[2],
    };

    int total_count = row_col_count * row_col_count;

/*    int x = 1 % row_col_count * tile_size;
    int y = 1 / row_col_count * tile_size;*/

    int random_color_index = random(0, 4);
    unsigned char current_color [] = { colors[random_color_index * 3], colors[random_color_index * 3 + 1], colors[random_color_index * 3 + 2] };

    /*image.draw_rectangle(x, y, x + tile_size, y + tile_size, current_color, 1.0); // Force use of transparency. -> Does not work. Always outputs 24bit PNGs.*/

    CImg<unsigned char> imgtext;
    unsigned char color = 1;

/*    imgtext.draw_text(10, 10, "Hello", &color, 0, 1, 40);*/ // Measure the text by drawing to an empty instance, so that the bounding box will be set automatically.*/
    image.draw_text((row_col_count / 2) - 50, 450, text, black, 1, 1, 25);

    // Save result image as PNG (libpng and GraphicsMagick are required).
    image.save_jpeg("example1.jpg");

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "Output took " << duration << "ms.";
    getchar();
}

int main()
{

    draw_text("example.jpg", "Alexa");
    return 0;
}