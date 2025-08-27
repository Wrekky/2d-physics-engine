#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
    Body* bigBox = new Body(BoxShape(200,200), Graphics::Width() / 2, Graphics::Height() / 2, 0.0);
    bigBox->rotation = 20;
    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2, Graphics::Height() - 50, 0.0);
    bodies.push_back(bigBox);
    bodies.push_back(floor);
    
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
            case SDL_KEYUP:
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Body* body = new Body(BoxShape(50, 50), x, y, 1.0);
                    bodies.push_back(body);
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    Graphics::ClearScreen(0xFF056263);
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

    for (auto body : bodies)
    {
        weight = Vec2(0.0, body->mass * gravity);
        body->AddForce(weight);
        body->AddForce(pushForce);
    }

    //apply forces
    for (auto body : bodies)
    {
        body->Update(deltaTime);
    }

    //check for a collision


    if(bodies.size() > 1) {
        for(int i = 0; i < bodies.size(); i++) {
            bodies[i]->isColliding = false;
        }
        for (int i = 0; i <= bodies.size() - 1; i++)
        {
            for (int j = i + 1; j < bodies.size(); j++)
            {
                Body *a = bodies[i];
                Body *b = bodies[j];
                Contact contact;
                if (CollisionDetection::IsColliding(a, b, contact))
                {
                    Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFFFFFF);
                    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFFFFFF);
                    Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFFFFFFFF);
                    a->isColliding = true;
                    b->isColliding = true;
                    //contact.ResolveCollision();
                }
            }
        }
    }


    //game logic (move above checking collision potentially)
    for (auto body : bodies)
    {
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape *circleShape = (CircleShape *)body->shape;
            if (body->position.x >= Graphics::Width() - circleShape->radius)
            {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -1;
            }
            else if (body->position.x <= 0 + circleShape->radius)
            {
                body->position.x = 0.0 + circleShape->radius;
                body->velocity.x *= -1;
            }

            if (body->position.y >= Graphics::Height() - circleShape->radius)
            {
                body->position.y = Graphics::Height() - circleShape->radius;
                body->velocity.y *= -1;
            }
            else if (body->position.y <= 0 + circleShape->radius)
            {
                body->position.y = 0.0 + circleShape->radius;
                body->velocity.y *= -1;
            }
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
    for (auto body : bodies)
    {
        Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape *circleShape = (CircleShape*)body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
        }
        else if(body->shape->GetType() == BOX) 
        {
            BoxShape *boxShape = (BoxShape*)body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);
        }
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for (auto body: bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}

void CreateBox() {

}