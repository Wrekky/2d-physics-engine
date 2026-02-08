#include "LightMapObject.h"
LightMapObject::LightMapObject(const Body& body) : body(body) {}
Shape* LightMapObject::GetShape() {
    return body.shape;
}
Vec2 LightMapObject::GetPosition() {
    return body.position;
}
float LightMapObject::GetRadius() {
    if (body.shape->GetType() == CIRCLE) {
        CircleShape* shape = (CircleShape*)body.shape;
        return shape->radius;
    }
    return NULL;
}
std::vector<Vec2> LightMapObject::GetWorldVertices() {
    if (body.shape->GetType() == BOX || body.shape->GetType() == POLYGON) {
        PolygonShape* shape = (PolygonShape*)body.shape;
        return shape->worldVertices;
    }
}