#include "Shape.h"

CircleShape::CircleShape(float radius) {
    this->radius = radius;
};
CircleShape::~CircleShape() {};
Shape* CircleShape::Clone() const {
    return new CircleShape(radius);
};
ShapeType CircleShape::GetType() const {
    return CIRCLE;
};

float CircleShape::GetMomentOfInertia() const {
    return (1/2) * (radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
    // TODO: ...
};
PolygonShape::~PolygonShape() {};
Shape* PolygonShape::Clone() const {
    return new PolygonShape(localVertices);
};
ShapeType PolygonShape::GetType() const {
    return POLYGON;
};

float PolygonShape::GetMomentOfInertia() const {
    //TODO: ...
    return 0;
}

Vec2 PolygonShape::EdgeAt(int index) const {
    int currVertex = index;
    int nextVertex = (index + 1) % worldVertices.size(); //trick to wrap back to the start
    
    return worldVertices[nextVertex] - worldVertices[currVertex];
}
void PolygonShape::UpdateVertices(float rotation, const Vec2& position) {
    for(int i = 0; i < localVertices.size(); i++) {
        worldVertices[i] = localVertices[i].Rotate(rotation);
        worldVertices[i] += position;
    }
}

BoxShape::BoxShape(float width, float height) {
    this->width = width;
    this->height = height;
    //loading vertices based on local origin.
    localVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2(width / 2.0, -height / 2.0));
    localVertices.push_back(Vec2(width / 2.0, height / 2.0));
    localVertices.push_back(Vec2(-width / 2.0, height / 2.0));

    worldVertices.push_back(Vec2(-width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2(width / 2.0, -height / 2.0));
    worldVertices.push_back(Vec2(width / 2.0, height / 2.0));
    worldVertices.push_back(Vec2(-width / 2.0, height / 2.0));
};
BoxShape::~BoxShape() {};
Shape* BoxShape::Clone() const {
    return new BoxShape(width, height);
};
ShapeType BoxShape::GetType() const {
    return BOX;
};

float BoxShape::GetMomentOfInertia() const {
    return (0.083333) * (width * width + height * height);
}