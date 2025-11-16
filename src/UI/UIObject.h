#ifndef UIOBJECT_H
#define UIOBJECT_H
#include "Vec2.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
struct UIObject
{
    Vec2 position;
    Vec2 size;

    Uint32 color;
    SDL_Texture* texture = nullptr;
    
    virtual void Draw();

    UIObject() = default;
    UIObject()~ = default;
};

struct TextButton : public UIObject {
    TextButton(String text, Vec2 pos, Vec2 size);
    void Draw() override;
};
#endif