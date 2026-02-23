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
    Vec2 furthestLeft;
    Vec2 furthestRight;
    Vec2 furthestUp;
    Vec2 furthestDown;
    int rayHitCount = 0;
    Vec2 currentNormal;
    float degreeTotal = DEGREE * beamSpread;
    float startPoint = direction - degreeTotal / 2;
    startPoint = clampDegree(startPoint);
    std::vector<Ray*> rays;
    std::vector<Ray*> bounceRays;
    Ray* tempRay;
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
        //Calculate how many different normals there are for each

        if (rayIntersect) {
            if (rayHitCount == 0) {
                furthestLeft = ray->endPos;
                furthestRight = ray->endPos;
                furthestDown = ray->endPos;
                furthestUp = ray->endPos;
                currentNormal = ray->bounceDir;
                tempRay = ray;
            }
            rayHitCount++;
            
            //better way?
            if (ray->endPos.x > furthestRight.x) {
                furthestRight = ray->endPos;
            }
            if (ray->endPos.x < furthestLeft.x) {
                furthestLeft = ray->endPos;
            }

            if (ray->endPos.y > furthestDown.y) {
                furthestDown = ray->endPos;
            }
            if (ray->endPos.y < furthestUp.y) {
                furthestUp = ray->endPos;
            }
            if (currentNormal != ray->bounceDir) {
                //need to set end pos, bounce dir
                //Ray* bouncePosRay = new Ray();
                //bouncePosRay->endPos = //inbetween all
                //fuck it just use one side for now.
            }
        }
    }
    //creating bounce rays:
    std::vector<Ray *> newRays;
    if (rayHitCount > 0) {
        //TODO: adjust intensity of color based on raycount
        Vec2 centerPoint;
        Vec2 p1, p2;
        float leftRightMag = std::abs((furthestLeft - furthestRight).Magnitude());
        float upDownMag = std::abs((furthestUp - furthestDown).Magnitude());
        if (leftRightMag > upDownMag) {
            p1 = furthestLeft;
            p2 = furthestRight;
        }
        else {
            p1 = furthestUp;
            p2 = furthestDown;
        }
        //Grab the center point between p1 and p2.
        centerPoint = ((p1-p2) / 2) + p2; //think this is the center point?
        // bounce stuff
        
        // fire 18 rays, 1 degree * 10
        Vec2 startPosition = centerPoint;
        Vec2 endPosition = (currentNormal * intensity) + startPosition;
        float rotatedDegreeRadians = 90 * DEGREE;
        for (int i = 0; i < 180; i++)
        {
            Ray *bounceRay = new Ray();
            float step = rotatedDegreeRadians - (DEGREE * i);
            step = clampDegree(step);
            bounceRay->endPos = endPosition - startPosition;
            bounceRay->endPos = bounceRay->endPos.Rotate(step);
            bounceRay->endPos += startPosition;

            // End pos should be segment normal
            bounceRay->position = startPosition;
            bounceRay->distance = intensity;
            // bounceRay->angle = currentAddDegreeBounce;
            for (auto obj : lightMap)
            {
                if (obj == tempRay->hitObject)
                {
                    continue;
                }
                RayIntersect(obj, bounceRay);
            }
            newRays.push_back(bounceRay);
            Graphics::DrawLine(bounceRay->position.x, bounceRay->position.y, bounceRay->endPos.x, bounceRay->endPos.y, color);
        }
    }
    currentRays.push_back(newRays);
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
                Vec2 bounceDir = ((c - d).Normal().UnitVector());//TODO: Include ray vector some how;
                ray->endPos = newEnd;
                ray->bounceDir = (bounceDir) * -1;
                oldDist = newDist;
            }
            ray->hitObject = obj;
            ray->hasHit = true;
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