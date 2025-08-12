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
    particle = new Particle(500, 100, 50);
    // TODO: setup objects in the scene
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
    //All physics code, TODO: split into a seperate function/class
    particle->velocity = Vec2(50.0 * deltaTime, 50.0  * deltaTime);
    particle->position += (particle->velocity);
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
    Graphics::DrawFillCircle(particle->position.x, particle->position.y, 10, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    // TODO: destroy all objects in the scene
    delete particle;
    Graphics::CloseWindow();
}