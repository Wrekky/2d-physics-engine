#include "Scene.h"
#include "../Physics/CollisionDetection.h"

void TitleScreen::Setup() {
    //TODO: setup static display menus
    FontSetup();
    world = new World(9.8 * PIXELS_PER_METER);

    TextButton *sceneOne = new TextButton("Scene one", defaultFont, Vec2(200, 200), Vec2(200, 100), SDL_Color{255,255,255,255}, 0xFFF51284, true);
    UIObjects.push_back(sceneOne);

    TextButton *sceneTwo = new TextButton("Scene two", defaultFont, Vec2(200, 400), Vec2(200, 100), SDL_Color{255,255,255,255}, 0xFFF51284, true);
    UIObjects.push_back(sceneTwo);

    TextButton *testScene = new TextButton("Test scene", defaultFont, Vec2(200, 600), Vec2(200, 100), SDL_Color{255,255,255,255}, 0xFFF51284, true);
    UIObjects.push_back(testScene);

    TextButton *textButton = new TextButton("test", defaultFont, Vec2(800, 800), Vec2(500, 200), SDL_Color{255,255,255,255}, 0xFFF51284, true);
    UIObjects.push_back(textButton);
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
            for (int i = 0; i < UIObjects.size(); i++) {
                if (!CollisionDetection::IsInside(Vec2(x, y), UIObjects[i])) {
                    UIObjects[i]->color = 0xFFF51284;
                }
                else {
                    UIObjects[i]->color = 0xFFF11214;
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);
            for (int i = 0; i < UIObjects.size(); i++) {
                if (CollisionDetection::IsInside(Vec2(x, y), UIObjects[i])) {
                    UIObjects[i]->color = 0xFFF54284;
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