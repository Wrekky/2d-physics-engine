#ifndef UIOBJECT_H
#define UIOBJECT_H
#include "../Utility/Vec2.h"
#include "../Text.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
struct UIObject
{
    Vec2 position;
    Vec2 size;

    Uint32 color;
    SDL_Texture* texture = nullptr;
    bool visible = false;
    bool clickable = false;

    virtual void Draw();

    virtual ~UIObject() = default;
};

struct TextButton : public UIObject {
    Text* text;
    SDL_Color textColor;
    TextButton(const char* text, TTF_Font* font, Vec2 pos, Vec2 size, SDL_Color textColor, Uint32 color, bool clickable);
    void Draw() override;
    //TODO add a click function which can be adjusted based on a scenes needs. try to make it work like how events work
};
#endif