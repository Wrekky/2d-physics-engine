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
    return (0.5) * (radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
    for (auto vertex: vertices) {
        localVertices.push_back(vertex);
        worldVertices.push_back(vertex);
    }
};
PolygonShape::~PolygonShape() {};
Shape* PolygonShape::Clone() const {
    return new PolygonShape(localVertices);
};
ShapeType PolygonShape::GetType() const {
    return POLYGON;
};

float PolygonShape::GetMomentOfInertia() const {
    //TODO:
    //Do this properly.
    return 5000;
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

float PolygonShape::FindMinSeparation(const PolygonShape* other, Vec2& axis, Vec2& point) const {
    float separation = std::numeric_limits<float>::lowest();
    for (int i = 0; i < worldVertices.size(); i++) {
        Vec2 va = worldVertices[i];
        Vec2 normal = EdgeAt(i).Normal();

        float minSep = std::numeric_limits<float>::max();

        Vec2 minVertex;

        for (int j = 0; j < other->worldVertices.size(); j++) {
            Vec2 vb = other->worldVertices[j];
            float proj = (vb - va).Dot(normal);
            if (minSep > proj) {
                minSep = proj;
                minVertex = vb;
            }
        }

        if (separation < minSep) {
            separation = minSep;
            axis = this->EdgeAt(i);
            point = minVertex;
        }
    }

    return separation;
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