#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>
#include <string>
bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    if (TTF_Init() == -1) {
        std::cout << "ttf init failed" << std::endl;
    }

    roboto = TTF_OpenFont("./assets/roboto.ttf", 36);
    if (!roboto)
    {
        std::cout << "load failed" << std::endl;
    }

    SDL_Color white = {255,255,255,255};
    objectCountText = new Text(500, 500, "testing font", roboto, white);
    textObjects.push_back(objectCountText);

    Body* bigCircle = new Body(CircleShape(100), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
    bigCircle->rotation = 1.4;
    bigCircle->restitution = 0.5;
    bodies.push_back(bigCircle);

    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
    floor->restitution = 0.2;
    bodies.push_back(floor);

    Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 50), 50, Graphics::Height() / 2, 0.0);
    floor->restitution = 0.2;
    bodies.push_back(leftWall);

    Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 50), Graphics::Width() - 50, Graphics::Height() / 2, 0.0);
    floor->restitution = 0.2;
    bodies.push_back(rightWall);

    Body* roof = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, 0 + 50, 0.0);
    floor->restitution = 0.2;
    bodies.push_back(roof);

    Body* bigBox = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
    bigBox->rotation = 1.4;
    bigBox->restitution = 0.5;
    bigBox->SetTexture("./assets/crate.png");
    bodies.push_back(bigBox);
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    int x, y;
    while (SDL_PollEvent(&event)) {
        
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_d) {
                    debug = !debug;
                }
                break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x, &y);
                bodies[0]->position = Vec2(x, y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&x, &y);
                //std::vector<Vec2> vertices = {Vec2(100, 100), Vec2(-100, 100), Vec2(-150, 0), Vec2(-100, -100), Vec2(100, -100), Vec2(150, 0)};
                Body *circle = new Body(CircleShape(50), x, y, 1.0);
                circle->restitution = 1.0;
                circle->SetTexture("./assets/basketball.png");
                bodies.push_back(circle);
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
    }

    //apply forces
    for (auto body : bodies)
    {
        body->Update(deltaTime);
    }

    //check for a collision

    for (int i = 0; i <= bodies.size() - 1; i++)
    {
        for (int j = i + 1; j < bodies.size(); j++)
        {
            Body *a = bodies[i];
            Body *b = bodies[j];
            a->isColliding = false;
            b->isColliding = false;
            Contact contact;
            if (CollisionDetection::IsColliding(a, b, contact))
            {
                contact.ResolveCollision();
                if (debug) {
                    Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                    Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFFFF00FF);
                    a->isColliding = true;
                    b->isColliding = true;
                }

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
            if (body->texture != nullptr && !debug) {
                Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius*2, body->rotation, body->texture);
            }
            else {
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
            }
        }
        else if(body->shape->GetType() == BOX) 
        {
            BoxShape *boxShape = (BoxShape*)body->shape;
            if (body->texture != nullptr && !debug) {
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            }
            else {
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);
            }
        }
        else if(body->shape->GetType() == POLYGON) 
        {
            PolygonShape *polygonShape = (PolygonShape*)body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, color);
        }
    }
    
    if (debug) {
        std::string objectCountString = "Object Count: " + std::to_string(bodies.size());
        objectCountText->ChangeText(objectCountString.c_str());
        int boxWidth = 300;
        int boxHeight = textObjects.size() * textObjects[0]->height;
        Graphics::DrawFillRect(0 + boxWidth / 2, 0 + boxHeight / 2, boxWidth, boxHeight, 0xFF33cc33);
        for (int i = 0; i < textObjects.size(); i++) {
            Graphics::DrawTexture(0 + (textObjects[i]->width / 2.0) + 5, i * textObjects[i]->height + (textObjects[i]->height / 2), textObjects[i]->width, textObjects[i]->height, 0, textObjects[i]->finishedTexture);
        }
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for (auto body : bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}

void CreateBox() {

}