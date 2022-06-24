#include <string>
#include "CImg.h"
#include <iostream>

/*#include <tgbot/tgbot.h>*/

using namespace std;
/*using namespace TgBot;*/
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

int main()
{
    CImg< unsigned char > image = CImg< unsigned char >("/Users/bodya/Downloads/Test/images/ex.png");
    draw_text(image, "Hello");
    return 0;
}