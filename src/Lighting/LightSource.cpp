#include "LightSource.h"
#include "../Utility/Constants.h"
LightSource::LightSource(Vec2 position, float direction, Uint32 color, float intensity, int bounces, int beamSpread) {
    this->position = position;
    this->direction = direction;
    this->color = color;
    this->intensity = intensity;
    this->bounces = bounces;
    this->beamSpread = beamSpread;
}

void LightSource::AddLightMapObject(LightMapObject* lightObject) {
    this->lightMap.push_back(lightObject);
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
        Ray* ray = new Ray();
        ray->distance = intensity;
        ray->position = position;
        ray->endPos = Vec2(endX, endY);
        for (auto obj : lightMap) {
            RayIntersect(obj, ray);
        }
        currentRays.push_back(ray);
        Graphics::DrawLine(ray->position.x, ray->position.y, ray->endPos.x, ray->endPos.y, color);
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

float orient(Vec2 a, Vec2 b, Vec2 c) {
    Vec2 ba;
    Vec2 ca;
    ba = b-a;
    ca = c-a;
    return  ba.Cross(ca);
}
bool LightSource::RayIntersect(LightMapObject* obj, Ray* ray) {
    Vec2 a = ray->position;
    Vec2 b = ray->endPos;
    //loop through all vertex combinations, return at first hit.
    std::vector<Vec2> worldVertices = obj->GetWorldVertices();
    float oldDist = Utils::distance(ray->position, ray->endPos);
    for (int i = 0; i < worldVertices.size(); i++) {
        int z = 0;
        if (i == worldVertices.size() - 1) {
            z = 0;
        }
        else {
            z = i + 1;
        }
        Vec2 c = worldVertices[i];
        Vec2 d = worldVertices[z];

        float oa = orient(c, d, a);
        float ob = orient(c, d, b);
        float oc = orient(a, b, c);
        float od = orient(a, b, d);
        
        if (oa * ob < 0 && oc * od < 0) {
            //TODO: break off into distance class, only call on edistance calculation per loop. seperate later...
            Vec2 newEnd = ((a * ob) - (b * oa)) / (ob-oa); 
            float newDist = Utils::distance(ray->position, newEnd);
            if (newDist < oldDist) {
                ray->endPos = newEnd;
                oldDist = newDist;
            }
        }
    }
    return true;
}

void LightSource::FillRays() {
    for (int i = 0; i < currentRays.size(); i++) {
        if (i == currentRays.size() - 1){
            continue;
        }
        int verticeA = i;
        int verticeB = i + 1;
        std::vector<Vec2> vertices;
        vertices.push_back(currentRays[verticeA]->position);
        vertices.push_back(currentRays[verticeA]->endPos);
        vertices.push_back(currentRays[verticeB]->endPos);
        Graphics::DrawFillPolygon(position.x, position.y, vertices, color);
    }
    //TODO: clear currentRays somewhere else. should be cleared everyframe in a lightsource.update function.    
    currentRays.clear();
}