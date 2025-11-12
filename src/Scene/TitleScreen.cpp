#include "Scene.h"
#include "../Physics/CollisionDetection.h"
void TitleScreen::Setup() {
    //TODO: setup static display menus
    FontSetup();
    world = new World(9.8 * PIXELS_PER_METER);
    Body *box = new Body(BoxShape(200, 100), 200, 200, 0.0);
    world->AddBody(box);
}
void TitleScreen::Input()
{
    SDL_Event event;
    int x, y;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
            break;
        case SDL_KEYUP:
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (CollisionDetection::IsInside(Vec2(x,y), world->GetBodies()[0])) {
                world->GetBodies()[0]->color = 0xFFF51284;
            }
            else {
                world->GetBodies()[0]->color = 0xFFF11214;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);
            if (CollisionDetection::IsInside(Vec2(x,y), world->GetBodies()[0])) {
                world->GetBodies()[0]->color = 0xFFF54284;
                switchScene = true;
                nextScene = 1;
            }
            break;
        }
    }
}

void TitleScreen::Update() {
    //might not need this
}