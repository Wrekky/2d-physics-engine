#ifndef LIGHTMAPOBJECT_H
#define LIGHTMAPOBJECT_H
#include "../Physics/Vec2.h"
#include "../Physics/Body.h"
class LightMapObject {
    public:    
        LightMapObject(const Body& body);
        Shape* GetShape();
        std::vector<Vec2> GetWorldVertices();
        Vec2 GetPosition();
        float GetRadius();
    private:
        const Body& body;
};
#endif