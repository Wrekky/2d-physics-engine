#include "Render.h"
#include "./Utility/Constants.h"
#include <iostream>
#include <string>
///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Render::RenderScene() {
    std::vector<Body*> bodies = scene->world->GetBodies();
    for (auto body : bodies)
    {
        Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape *circleShape = (CircleShape*)body->shape;
            if (body->texture != nullptr && !scene->debug) {
                Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius*2, body->rotation, body->texture);
            }
            else {
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
            }
        }
        else if(body->shape->GetType() == BOX) 
        {
            BoxShape *boxShape = (BoxShape*)body->shape;
            if (body->texture != nullptr && !scene->debug) {
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            }
            else if (body->color != NULL && !scene->debug) {
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, body->color);
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
    for (auto lightSource : scene->lightObjects) {
        //lightSource->FillRays();
    }
    for (auto uiObj : scene->UIObjects) {
        TextButton *textButton = (TextButton*)uiObj;
        
        textButton->Draw();
    }
    if (scene->debug) {
        if (scene->textObjects.size() > 0) {
                int boxWidth = 300;
                int boxHeight = scene->textObjects.size() * scene->textObjects[0]->height;
                Graphics::DrawFillRect(0 + boxWidth / 2, 0 + boxHeight / 2, boxWidth, boxHeight, 0xFF33cc33);
                for (int i = 0; i < scene->textObjects.size(); i++)
                {
                    Graphics::DrawTexture(0 + (scene->textObjects[i]->width / 2.0) + 5, i * scene->textObjects[i]->height + (scene->textObjects[i]->height / 2), scene->textObjects[i]->width, scene->textObjects[i]->height,
                                          0, scene->textObjects[i]->finishedTexture);
                }
        }
        for (int i = 0; i < scene->world->GetConstraints().size(); i++) {
            std::vector<Constraint *> constraints = scene->world->GetConstraints();
            Constraint *constraint = constraints[i];
            Vec2 pointA = constraint->a->LocalSpaceToWorldSpace(constraint->aPoint);
            Vec2 pointB = constraint->b->LocalSpaceToWorldSpace(constraint->bPoint);
            Graphics::DrawLine(pointA.x, pointA.y, pointB.x, pointB.y, 0xFF33cc33);
        }
    }

    Graphics::RenderFrame();
    Graphics::ClearScreen(0xFF056263);
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////

void Render::Destroy() {
    Graphics::CloseWindow();
}