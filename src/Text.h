#ifndef TEXT_H
#define TEXT_H

#include "Graphics.h"
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
struct Text {
    SDL_Texture* finishedTexture;
    int width;
    int height;
    int x;
    int y;
    Text(int x, int y, const char* text, TTF_Font* font, SDL_Color color);
    ~Text() = default;
};
#endif