#include "LightSource.h"
#include "../Utility/Constants.h"
#include "iostream"
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
    std::vector<Ray*> rays;
    for (int i = 0; i < beamSpread; i++) {
        float currentAddDegree = startPoint + (i * DEGREE);
        currentAddDegree = clampDegree(currentAddDegree);
        float endX = position.x + intensity * cos(currentAddDegree);
        float endY = position.y + intensity * sin(currentAddDegree);
        Ray* ray = new Ray();
        ray->distance = intensity;
        ray->position = position;
        ray->endPos = Vec2(endX, endY);
        ray->angle = currentAddDegree;
        bool rayIntersect = false;
        for (auto obj : lightMap) {
            rayIntersect = RayIntersect(obj, ray);
        }
        rays.push_back(ray);
        Graphics::DrawLine(ray->position.x, ray->position.y, ray->endPos.x, ray->endPos.y, color);
        //bounce logic (just one bounce for now):
        if (rayIntersect)
        {
            std::vector<Ray*> newRays;
            // fire 18 rays, 1 degree * 10
            for (int i = 0; i < 180; i += 60)
            {
                Ray *bounceRay = new Ray();
                //End pos should be segment normal
                bounceRay->position = ray->endPos;
                bounceRay->endPos = (ray->bounceDir * 100) + bounceRay->position;
                std::cout << bounceRay->position.x << " : " << bounceRay->position.y << std::endl;
                std::cout << bounceRay->endPos.x << " : " << bounceRay->endPos.y << std::endl;
                bounceRay->distance = intensity;
                //bounceRay->angle = currentAddDegreeBounce;
                //Graphics::DrawLine(bounceRay->position.x, bounceRay->position.y, bounceRay->endPos.x, bounceRay->endPos.y, color);
                for (auto obj : lightMap)
                {
                    rayIntersect = RayIntersect(obj, bounceRay);
                }
                newRays.push_back(bounceRay);
                Graphics::DrawLine(bounceRay->position.x, bounceRay->position.y, bounceRay->endPos.x, bounceRay->endPos.y, color);
            }
            currentRays.push_back(newRays);
            // Dont add to currentRays vector yet, drawing inbetween rays will not work.
            // TODO: Need some function to calculate bounce direction. Simplifying to -x or -y doesnt work.
            //
        }
    }
    currentRays.push_back(rays);
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
    bool result = false;
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
                Vec2 bounceDir = ((c - d).Normal().UnitVector()) 
                    + 
                    (((newEnd - ray->position)).UnitVector());
                ray->endPos = newEnd;
                ray->bounceDir = (bounceDir / 2) * -1;
                oldDist = newDist;
            }
            result = true;
        }
    }
    return result;
}

void LightSource::FillRays() {
    for (auto rays : currentRays) {
            for (int i = 0; i < rays.size(); i++) {
                if (i == rays.size() - 1)
                {
                    continue;
                }
                int verticeA = i;
                int verticeB = i + 1;
                std::vector<Vec2> vertices;
                vertices.push_back(rays[verticeA]->position);
                vertices.push_back(rays[verticeA]->endPos);
                vertices.push_back(rays[verticeB]->endPos);
                Graphics::DrawFillPolygon(position.x, position.y, vertices, color);
            }
            rays.clear();//maybewillcrash
    }
    //TODO: clear currentRays somewhere else. should be cleared everyframe in a lightsource.update function.    
    currentRays.clear();
}