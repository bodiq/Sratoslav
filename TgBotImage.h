#pragma once

#include <string>
#include "CImg.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <tgbot/tgbot.h>

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

void draw_text(cimg_library::CImg< unsigned char > &image, const std::string &text, const std::string &col)
{
    int tile_size = image.width();
    int row_col_count = image.height();

    std::cout << col << std::endl;
    const unsigned char *text_color = color(col);

    image.draw_text((tile_size / 2) - (tile_size / 100 * 10), (row_col_count / 2) + (row_col_count / 100 * 40), text.c_str(), text_color, 1, 1, 30);
    image.save_jpeg("/Users/bodya/Downloads/Test/images/ex1.jpg");
}
