#include "LightSource.h"
#include "../Physics/Constants.h"
LightSource::LightSource(Vec2 position, float direction, Uint32 color, float intensity, int bounces, int beamSpread) {
    this->position = position;
    this->direction = direction;
    this->color = color;
    this->intensity = intensity;
    this->bounces = bounces;
    this->beamSpread = beamSpread;
}

void LightSource::ShootRays() {
    float degreeTotal = DEGREE * beamSpread;
    float startPoint = direction - degreeTotal / 2;
    startPoint = clampDegree(startPoint);
    for (int i = 0; i < beamSpread; i++) {
        
        float currentAddDegree = startPoint + (i * DEGREE);
        currentAddDegree = clampDegree(currentAddDegree);
        float endX = position.x + intensity * cos(currentAddDegree);
        float endY = position.y + intensity * sin(currentAddDegree);
        Vec2 endPointRay = Vec2(endX, endY);
        Ray ray = Ray();
        ray.distance = intensity;
        ray.position = position;
        ray.endPos = Vec2(endX, endY);
        Graphics::DrawLine(this->position.x, this->position.y, endPointRay.x, endPointRay.y, color);
    }
}

float LightSource::clampDegree(float degree) {
    if (degree > TWO_PI) {
        degree = degree - TWO_PI;
        clampDegree(degree);
    }
    else if (degree < 0) {
        degree = degree + TWO_PI;
        clampDegree(degree);
    }
    return degree;
}

bool LightSource::RayIntersect(LightMapObject* obj, Ray ray) {
    Vec2 a = ray.position;
    Vec2 b = ray.endPos;
    //loop through all vertex combinations, return at first hit.
    std::vector<Vec2> worldVertices = obj->GetWorldVertices();
    for (int i = 0; i < worldVertices.size(); i++) {
        int z = 0;
        if (i == worldVertices.size()) {
            z = 0;
        }
        else {
            z = i + 1;
        }
        Vec2 c = worldVertices[i];
        Vec2 d = worldVertices[z];

        Vec2 r = b - a;
        Vec2 s = d - c;

        float t = ((c - a).Cross(s)) / (r.Cross(s));
        float u = ((c - a).Cross(s)) / (r.Cross(s));
        
        if ((0 < t && t < 1) && (0 < u && u < 1)) {
            return true;
            //return a + (r*t)
        }
    }
}