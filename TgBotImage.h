#pragma once

#include <string>
#include "CImg.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <tgbot/tgbot.h>



void draw_text(cimg_library::CImg< unsigned char > &image, const char *text)
{
    int tile_size = image.width();
    int row_col_count = image.height();

    unsigned char cyan[] = { 0, 255, 255 };
    unsigned char black[] = { 0, 0, 0 };
    unsigned char yellow[] = { 255, 255, 0 };
    unsigned char red[] = { 255, 0, 0 };
    unsigned char green[] = { 0, 255, 0 };
    unsigned char orange[] = { 255, 165, 0 };
    unsigned char blue[] = { 0,0,255 };

    /*image.draw_text((row_col_count / 2) - 50, 450, text, black, 1, 1, 25);*/
    image.draw_text((tile_size / 2) - (tile_size / 100 * 10), (row_col_count / 2) + (row_col_count / 100 * 40), text, black, 1, 1, 30);
    image.save_jpeg("/Users/bodya/Downloads/Test/images/ex1.jpg");
}