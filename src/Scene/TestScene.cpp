#include "Scene.h"
#include "../Physics/CollisionDetection.h"

void TestScene::Setup() {
    //TODO: setup static display menus
    FontSetup();
    world = new World(9.8 * PIXELS_PER_METER);

    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
    world->AddBody(floor);

    Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 50), 50, Graphics::Height() / 2, 0.0);
    world->AddBody(leftWall);

    Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 50), Graphics::Width() - 50, Graphics::Height() / 2, 0.0);
    world->AddBody(rightWall);

    Body* a = new Body(BoxShape(200, 200), Graphics::Width()/ 2.0, Graphics::Height() / 2.0, 0.0);
    Body* b = new Body(BoxShape(200, 200), 300, 0, 0.0);
    a->rotation = 180;
    b->rotation = 180;
    world->AddBody(a);
    world->AddBody(b);
    Vec2 position = Vec2(Graphics::Width() / 2, Graphics::Height() / 4);
    LightSource* newLight = new LightSource(position, 3.14 / 2, 0x33FF0000, 500, 1, 180);
    lightObjects.push_back(newLight);
    LightSource* newLight2 = new LightSource(Vec2(position.x + 200, position.y), 3.14 / 2, 0x330000FF, 500, 1, 180);
    lightObjects.push_back(newLight2);
    for (int i = 0; i < world->GetBodies().size(); i++) {
        LightMapObject* newLightMapObject = new LightMapObject(*world->GetBodies()[i]);
        newLight->AddLightMapObject(newLightMapObject);
        newLight2->AddLightMapObject(newLightMapObject);
    }
    //manual for now

}
void TestScene::Input()
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
            if (event.key.keysym.sym == SDLK_d)
                debug = !debug;
                world->debug = debug;
            break;
        case SDL_KEYUP:
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                SDL_GetMouseState(&x, &y);
                Body* ball = new Body(CircleShape(64), x, y, 1.0);
                ball->SetTexture("./assets/basketball.png");
                ball->restitution = 0.7;
                world->AddBody(ball);
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                SDL_GetMouseState(&x, &y);
                Body* box = new Body(BoxShape(140, 140), x, y, 1.0);
                box->SetTexture("./assets/crate.png");
                box->restitution = 0.2;
                world->AddBody(box);
            }
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            Body* box = world->GetBodies()[4];
            box->position.x = x;
            box->position.y = y;
            break;
        }
    }
}

void TestScene::Update() {
    static int timePreviousFrame;

    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);;
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016) {
        //If it pauses for whatever reason and deltaTime has a large gap.
        deltaTime = 0.016;
    }

    timePreviousFrame = SDL_GetTicks();
    world->Update(deltaTime);
    for (auto light : lightObjects) {
        light->ShootRays();
    }
}