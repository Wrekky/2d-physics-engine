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
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&x, &y);
                Body *box = new Body(BoxShape(200, 200), x, y, 1.0);
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
}

void SceneOne::Setup() {
    FontSetup();
    world = new World(9.8 * PIXELS_PER_METER);

    //add two rigid bodies
    //ragdoll test
    Body* head = new Body(CircleShape(25), 0.0, 0.0, 0.0f);
    Body* torso = new Body(BoxShape(50,100), 0.0, 0.0, 1.0f);
    Body* leftArm = new Body(BoxShape(15, 70), 0.0, 0.0, 1.0f);
    Body* rightArm = new Body(BoxShape(15, 70), 0.0, 0.0, 1.0f);
    Body* leftLeg = new Body(BoxShape(20, 90), 0.0, 0.0, 1.0f);
    Body* rightLeg = new Body(BoxShape(20, 90), 0.0, 0.0, 1.0f);
    world->AddBody(head);
    world->AddBody(torso);
    world->AddBody(leftArm);
    world->AddBody(rightArm);
    world->AddBody(leftLeg);
    world->AddBody(rightLeg);

    //constraint between head and torso
    JointConstraint* ht = new JointConstraint(head, torso, Vec2(torso->position.x, head->position.y + 25));
    //constraint between torso and left arm
    JointConstraint* tla = new JointConstraint(torso, leftArm, Vec2(25.0, 50.0));
    //constraint between torso and right arm
    JointConstraint* tra = new JointConstraint(torso, rightArm, Vec2(-25.0, 50.0));
    //constraint between torso and left leg
    JointConstraint* tll = new JointConstraint(torso, leftLeg, Vec2(-8.0, -45.0));
    //constraint between torso and right leg
    JointConstraint* trl = new JointConstraint(torso, rightLeg, Vec2(8.0, -45.0));

    world->AddConstraint(ht);
    world->AddConstraint(tla);
    world->AddConstraint(tra);
    world->AddConstraint(tll);
    world->AddConstraint(trl);
    //Body* a = new Body(CircleShape(20), Graphics::Width() / 2.0, Graphics::Height() / 2, 0.0f);
    // world->AddBody(a);
    // for (int i = 1; i < 5; i++) {
    //     Body* b = new Body(CircleShape(20), a->position.x - 20 * i, a->position.y + (100 * i), 1.0f);
    //     world->AddBody(b);
    // }

    // for (int i = 1; i < world->GetBodies().size(); i++) {
    //         //add a joint constraint
    //         Body* a = world->GetBodies()[i - 1];
    //         Body* b = world->GetBodies()[i];
    //         JointConstraint* joint = new JointConstraint(a, b, a->position);
    //         world->AddConstraint(joint);
    // }
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
                world->GetBodies()[0]->position = Vec2(x, y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&x, &y);
                Body *box = new Body(BoxShape(50, 50), x, y, 1.0);
                box->restitution = 0.1;
                world->AddBody(box);
                break;
        }
    }
}