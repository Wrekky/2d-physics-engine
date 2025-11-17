#include "UIObject.h"
//TODO implement text button stuff
TextButton::TextButton(const char* textString, TTF_Font* font, Vec2 position, Vec2 size, SDL_Color textColor, Uint32 color, bool clickable) {
    //Needs to do:
    //Create text object
    this->text = new Text(0, 0, textString, font, textColor);
    this->clickable = clickable;
    this->size = size;
    this->position = position;
    this->textColor = textColor;
    this->color = color;

    //all objects are drawn with the center in mind already, text should be centered.
    text->x = position.x;
    text->y = position.y;
}

void UIObject::Draw() {
    //
}

void TextButton::Draw() {
    Graphics::DrawFillRect(position.x, position.y, size.x, size.y, color);
    Graphics::DrawTexture(text->x, text->y, text->width, text->height, 0.0, text->finishedTexture);
}