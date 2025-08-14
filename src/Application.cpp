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

    Particle* smallBall = new Particle(100,50,1.0);
    smallBall->radius = 4;

    Particle* bigBall = new Particle(50,100,3.0);
    bigBall->radius = 8;

    particles.push_back(smallBall);
    particles.push_back(bigBall);
    // TODO: setup objects in the scene
    water.x = 0;
    water.y = Graphics::Height() / 2;
    water.w = Graphics::Width();
    water.h = Graphics::Height() / 2;
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
                    Particle* particle = new Particle(x, y, 1.0);
                    particle->radius = 5;
                    particles.push_back(particle);
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
    for (auto particle : particles)
    {
        weight = Vec2(0.0, particle->mass * gravity);
        particle->AddForce(weight);
        Vec2 friction = Force::GenerateFrictionForce(*particle, 10 * PIXELS_PER_METER);
        particle->AddForce(friction);
        particle->AddForce(pushForce);
        if (particle->position.y > water.y) {
            Vec2 drag = Force::GenerateDragForce(*particle, 0.01);
            particle->AddForce(drag);
        }
        else { // only  apply wind when above water
            particle->AddForce(wind);
        }
    }

    for (auto particle : particles)
    {
        particle->Integrate(deltaTime);
    }

    //game logic
    for (auto particle : particles)
    {
        if (particle->position.x >= Graphics::Width() - particle->radius)
        {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -1;
        }
        else if (particle->position.x <= 0 + particle->radius)
        {
            particle->position.x = 0.0 + particle->radius;
            particle->velocity.x *= -1;
        }

        if (particle->position.y >= Graphics::Height() - particle->radius)
        {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -1;
        }
        else if (particle->position.y <= 0 + particle->radius)
        {
            particle->position.y = 0.0 + particle->radius;
            particle->velocity.y *= -1;
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
    Graphics::DrawFillRect(water.x + water.w / 2, water.y + water.h / 2, water.w, water.h, 0xFFEB6134);
    for (auto particle : particles)
    {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for (auto particle: particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}