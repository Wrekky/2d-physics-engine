#include "Scene.h"

bool Scene::IsRunning() {
    return running;
}

void Scene::FontSetup() {
    if (TTF_Init() == -1) 
        std::cout << "ttf init failed" << std::endl;

    defaultFont = TTF_OpenFont("./assets/roboto.ttf", 36);

    if (!defaultFont)
        std::cout << "load failed" << std::endl;
}

void Scene::Setup() {
    FontSetup();
    //Default scene should be the menu screen but in this case it will be the collision test scene.

    world = new World(9.8 * PIXELS_PER_METER);
    SDL_Color white = {255,255,255,255};
    Text* objectCountText = new Text(500, 500, "testing font", defaultFont, white);
    textObjects.push_back(objectCountText);

    Body* bigCircle = new Body(CircleShape(100), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
    bigCircle->rotation = 1.4;
    bigCircle->restitution = 0.5;
    world->AddBody(bigCircle);

    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
    floor->restitution = 0.2;
    world->AddBody(floor);

    Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 50), 50, Graphics::Height() / 2, 0.0);
    floor->restitution = 0.2;
    world->AddBody(leftWall);

    Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 50), Graphics::Width() - 50, Graphics::Height() / 2, 0.0);
    floor->restitution = 0.2;
    world->AddBody(rightWall);

    Body* roof = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, 0 + 50, 0.0);
    floor->restitution = 0.2;
    world->AddBody(roof);

    Body* bigBox = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
    bigBox->rotation = 1.4;
    bigBox->restitution = 0.5;
    bigBox->SetTexture("./assets/crate.png");
    world->AddBody(bigBox);
}

void Scene::Input() {
    SDL_Event event;
    int x, y;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    switchScene = true;
                    nextScene = 0;
                if (event.key.keysym.sym == SDLK_d) {
                    debug = !debug;
                }
                break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x, &y);
                world->GetBodies()[0]->position = Vec2(x, y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&x, &y);
                Body *box = new Body(BoxShape(50, 50), x, y, 1.0);
                box->restitution = 0.1;
                box->SetTexture("./assets/crate.png");
                world->AddBody(box);
                break;
        }
    }
}

void Scene::Update() {
    //world stuff, order is very important.
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

    //ui stuff
    if (debug) {
        std::string objectCountString = "Object Count: " + std::to_string(world->GetBodies().size());
        textObjects[0]->ChangeText(objectCountString.c_str());
    }
}

void SceneOne::Setup() {
    FontSetup();
    world = new World(9.8 * PIXELS_PER_METER);

    //add two rigid bodies
    Body* a = new Body(CircleShape(20), Graphics::Width() / 2.0, Graphics::Height() / 2, 0.0f);
    world->AddBody(a);
    for (int i = 1; i < 5; i++) {
        Body* b = new Body(CircleShape(20), a->position.x - 20 * i, a->position.y + (100 * i), 1.0f);
        world->AddBody(b);
    }

    for (int i = 1; i < world->GetBodies().size(); i++) {
            //add a joint constraint
            Body* a = world->GetBodies()[i - 1];
            Body* b = world->GetBodies()[i];
            JointConstraint* joint = new JointConstraint(a, b, a->position);
            world->AddConstraint(joint);
    }
}

void SceneOne::Update() {
    //world stuff, order is very important.
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

    //ui stuff
    if (debug) {
        std::string objectCountString = "Object Count: " + std::to_string(world->GetBodies().size());
        textObjects[0]->ChangeText(objectCountString.c_str());
    }
}

void SceneOne::Input() {
    SDL_Event event;
    int x, y;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    switchScene = true;
                    nextScene = 0;
                if (event.key.keysym.sym == SDLK_d) {
                    debug = !debug;
                }
                break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x, &y);
                //world->GetBodies()[0]->position = Vec2(x, y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&x, &y);
                Body *box = new Body(BoxShape(50, 50), x, y, 1.0);
                box->restitution = 0.1;
                box->SetTexture("./assets/crate.png");
                world->AddBody(box);
                break;
        }
    }
}