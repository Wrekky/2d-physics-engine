#include "Application.h"
#include "./Physics/Constants.h"

#include <iostream>

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    Body* smallBall = new Body(100,50,1.0);
    smallBall->radius = 4;

    Body* bigBall = new Body(50,100,3.0);
    bigBall->radius = 8;

    bodys.push_back(smallBall);
    bodys.push_back(bigBall);
    // TODO: setup objects in the scene

    //chain body setup
    for(int i = 0; i < 10; i++) {
        Body* chain = new Body(500,500,3.0);
        chain->radius = 8;
        bodys.push_back(chain);
        chainBodys.push_back(chain);
    }
    //box body setup
    for(int i = 0; i < 4; i++) {
        Body* body = new Body(500 + (i * 10),500,3.0);
        body->radius = 8;
        bodys.push_back(body);
        boxBodys.push_back(body);
    }

    water.x = 0;
    water.y = Graphics::Height() / 2;
    water.w = Graphics::Width();
    water.h = Graphics::Height() / 2;
    anchor = Vec2(Graphics::Width()/2, 30);
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Body* body = new Body(x, y, 1.0);
                    body->radius = 5;
                    bodys.push_back(body);
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // TODO: update all objects in the scene
    //Check fps
    static int timePreviousFrame;
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016) {
        //If it pauses for whatever reason and deltaTime has a large gap for whatever reason
        deltaTime = 0.016;
    }
    int physicsTime = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);

    //apply physics
    Vec2 wind(10.0 * PIXELS_PER_METER, 0.0);//adding forces
    float gravity = 9.8 * PIXELS_PER_METER;
    Vec2 weight(0.0, 0.0);


    //adding a spring to the top of the screen to the smaller ball
    Vec2 springForce = Force::GenerateSpringForce(*bodys[0], anchor, 200, 40);
    bodys[0]->AddForce(springForce);
    for (auto body : bodys)
    {
        Force::GenerateChainBoxForces(100, 50, boxBodys);
        Force::GenerateChainForces(Vec2(500,500), 20, 20, chainBodys);
        weight = Vec2(0.0, body->mass * gravity);
        body->AddForce(weight);
        Vec2 friction = Force::GenerateFrictionForce(*body, 10 * PIXELS_PER_METER);
        body->AddForce(friction);
        body->AddForce(pushForce);
        if (body->position.y > water.y) {
            //Vec2 drag = Force::GenerateDragForce(*body, 0.01);
            //body->AddForce(drag);
        }
        else { // only  apply wind when above water
            //body->AddForce(wind);
        }
    }

    for (auto body : bodys)
    {
        body->Integrate(deltaTime);
    }

    //game logic
    for (auto body : bodys)
    {
        if (body->position.x >= Graphics::Width() - body->radius)
        {
            body->position.x = Graphics::Width() - body->radius;
            body->velocity.x *= -1;
        }
        else if (body->position.x <= 0 + body->radius)
        {
            body->position.x = 0.0 + body->radius;
            body->velocity.x *= -1;
        }

        if (body->position.y >= Graphics::Height() - body->radius)
        {
            body->position.y = Graphics::Height() - body->radius;
            body->velocity.y *= -1;
        }
        else if (body->position.y <= 0 + body->radius)
        {
            body->position.y = 0.0 + body->radius;
            body->velocity.y *= -1;
        }
    }

    /////////////////////////////////////////////
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);;
    timePreviousFrame = SDL_GetTicks();
    physicsTime = (physicsTime - timeToWait);
    
    if (physicsTime > MILLISECS_PER_FRAME) {
        std::cout << "Took: " << physicsTime << " miliseconds to calculate physics" << std::endl;
    }
    
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    //Graphics::DrawFillRect(water.x + water.w / 2, water.y + water.h / 2, water.w, water.h, 0xFFEB6134);
    for (auto body : bodys)
    {
        Graphics::DrawFillCircle(body->position.x, body->position.y, body->radius, 0xFFFFFFFF);
    }
    //drawing chain
    Graphics::DrawFillCircle(500, 500, 6, 0xFFFFFFFF);
    Graphics::DrawLine(500, 500, chainBodys[0]->position.x, chainBodys[0]->position.y, 0xFFFFFFFF);
    for (int i = 1; i < chainBodys.size(); i++) {
        Graphics::DrawLine(chainBodys[i]->position.x, chainBodys[i]->position.y, 
            chainBodys[i - 1]->position.x, chainBodys[i - 1]->position.y, 
            0xFFFFFFFF);
    }
    //drawing chain box
    for (int x = 0; x < boxBodys.size(); x++) {
        for (int y = 0; y < boxBodys.size(); y++) {
            if (x != y) {
                Graphics::DrawLine(boxBodys[x]->position.x, boxBodys[x]->position.y, 
                    boxBodys[y]->position.x, boxBodys[y]->position.y,
                0xFFFFFFFF);
            }
        }
    }
    Graphics::DrawFillCircle(anchor.x, anchor.y, 4, 0xFFFFFFFF);
    Graphics::DrawLine(anchor.x, anchor.y, bodys[0]->position.x, bodys[0]->position.y, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for (auto body: bodys) {
        delete body;
    }
    Graphics::CloseWindow();
}

void CreateBox() {

}