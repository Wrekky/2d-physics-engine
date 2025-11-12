#include "Scene.h"
#include "../Physics/CollisionDetection.h"
void TitleScreen::Setup() {
    //TODO: setup static display menus
    FontSetup();
    world = new World(9.8 * PIXELS_PER_METER);
    Body *sceneOne = new Body(BoxShape(200, 100), 200, 200, 0.0);
    world->AddBody(sceneOne);

    Body *sceneTwo = new Body(BoxShape(200, 100), 200, 400, 0.0);
    world->AddBody(sceneTwo);
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
            for (int i = 0; i < world->GetBodies().size(); i++) {
                if (!CollisionDetection::IsInside(Vec2(x, y), world->GetBodies()[i])) {
                    world->GetBodies()[i]->color = 0xFFF51284;
                }
                else {
                    world->GetBodies()[i]->color = 0xFFF11214;
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);
            for (int i = 0; i < world->GetBodies().size(); i++) {
                if (CollisionDetection::IsInside(Vec2(x, y), world->GetBodies()[i])) {
                    world->GetBodies()[i]->color = 0xFFF54284;
                    switchScene = true;
                    nextScene = i + 1;//for now
                }
            }

            break;
        }
    }
}

void TitleScreen::Update() {
    //might not need this
}