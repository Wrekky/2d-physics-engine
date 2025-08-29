#include "Text.h"
#include <iostream>
Text::Text(int x, int y, const char* text, TTF_Font* font, SDL_Color color) {
    this->x = x;
    this->y = y;
    this->font = font;
    this->color = color;
    ChangeText(text);
};

void Text::ChangeText(const char* text) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);

    if (!textSurface) {
        std::cout << "text surface failed" << std::endl;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, textSurface);

    if (!textTexture) {
        std:: cout << "text failed" << std::endl;
    }
    std::cout << "this is hitting..." << std::endl;
    this->width = textSurface->w;
    this->height = textSurface->h;

    finishedTexture = textTexture;
    SDL_FreeSurface(textSurface); 
}

