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

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
    // TODO: ...
};
PolygonShape::~PolygonShape() {};
Shape* PolygonShape::Clone() const {
    return new PolygonShape(vertices);
};
ShapeType PolygonShape::GetType() const {
    return POLYGON;
};

BoxShape::BoxShape(float width, float height) {
    // TODO: ...
};
BoxShape::~BoxShape() {};
Shape* BoxShape::Clone() const {
    return new BoxShape(width, height);
};
ShapeType BoxShape::GetType() const {
    return BOX;
};


