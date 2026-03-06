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
    int rayHitCount = 0;
    Vec2 currentNormal;
    float degreeTotal = DEGREE * beamSpread;
    float startPoint = direction - degreeTotal / 2;
    startPoint = clampDegree(startPoint);
    std::vector<Ray*> rays;
    std::vector<BounceInfo*> bounceInformation;
    int iterationCount = -1;
    for (float i = 0; i <= 180; i+=0.1) {
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
            bool truefalse = RayIntersect(obj, ray);
            if (!rayIntersect) {
                rayIntersect = truefalse;
            }
        }
        rays.push_back(ray);
        //Graphics::DrawLine(ray->position.x, ray->position.y, ray->endPos.x, ray->endPos.y, color);
        //bounce logic (just one bounce for now):
        //Calculate how many different normals there are for each
        if (rayIntersect) {
            if (rayHitCount == 0 || currentNormal != ray->bounceDir || ray->hitObject != bounceInformation[iterationCount]->currentRay->hitObject) {
                BounceInfo* bounceInfo = new BounceInfo();
                bounceInfo->furthestLeft = ray->endPos;
                bounceInfo->furthestRight = ray->endPos;
                bounceInfo->furthestDown = ray->endPos;
                bounceInfo->furthestUp = ray->endPos;
                bounceInfo->currentNormal = ray->bounceDir;
                bounceInfo->currentRay = ray;
                currentNormal = ray->bounceDir;
                bounceInformation.push_back(bounceInfo);
                iterationCount++;
            }//TODO: some logic bug with the ray center point being created. dont see it at the moment
            rayHitCount++;
            if (ray->endPos.x > bounceInformation[iterationCount]->furthestRight.x) {
                bounceInformation[iterationCount]->furthestRight = ray->endPos;
            }
            if (ray->endPos.x < bounceInformation[iterationCount]->furthestLeft.x) {
                bounceInformation[iterationCount]->furthestLeft = ray->endPos;
            }

            if (ray->endPos.y > bounceInformation[iterationCount]->furthestDown.y) {
                bounceInformation[iterationCount]->furthestDown = ray->endPos;
            }
            if (ray->endPos.y < bounceInformation[iterationCount]->furthestUp.y) {
                bounceInformation[iterationCount]->furthestUp = ray->endPos;
            }
        }
    }
    //creating bounce rays:
    //std::cout << bounceInformation.size() << std::endl;
    for (auto bounceInfo : bounceInformation)
    {
        std::vector<Ray *> newRays;
        if (rayHitCount > 0)
        {
            // TODO: adjust intensity of color based on raycount
            Vec2 centerPoint;
            Vec2 p1, p2;
            Vec2 furthestLeft = bounceInfo->furthestLeft;
            Vec2 furthestRight = bounceInfo->furthestRight;
            Vec2 furthestUp = bounceInfo->furthestUp;
            Vec2 furthestDown = bounceInfo->furthestDown;
            currentNormal = bounceInfo->currentNormal;
            float leftRightMag = std::abs((furthestLeft - furthestRight).Magnitude());
            float upDownMag = std::abs((furthestUp - furthestDown).Magnitude());
            if (leftRightMag > upDownMag)
            {
                p1 = furthestLeft;
                p2 = furthestRight;
            }
            else
            {
                p1 = furthestUp;
                p2 = furthestDown;
            }
            // Grab the center point between p1 and p2.
            centerPoint = ((p1 - p2) / 2) + p2; // think this is the center point?
            // bounce stuff

            // fire 18 rays, 1 degree * 10
            Vec2 startPosition = centerPoint;
            Vec2 endPosition = (currentNormal * intensity) + startPosition;
            float rotatedDegreeRadians = 90 * DEGREE;
            for (float i = 0; i <= 180; i+=0.1)
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
                    if (obj == bounceInfo->currentRay->hitObject)
                    {
                        continue;
                    }
                    RayIntersect(obj, bounceRay);
                }
                newRays.push_back(bounceRay);
                //Graphics::DrawLine(bounceRay->position.x, bounceRay->position.y, bounceRay->endPos.x, bounceRay->endPos.y, color);
            }
        }
        currentRays.push_back(newRays);
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
            if (newDist < oldDist)
            {
                // TODO: Include ray vector some how;
                ray->hasHit = true;
                result = true;
                ray->endPos = newEnd;
                oldDist = newDist;
                Vec2 bounceDir = ((c - d).Normal().UnitVector());
                ray->bounceDir = (bounceDir) * -1;
                ray->hitObject = obj;
            }
        }
    }
    return result;
}

//minDist controls how complex the polygon is, higher values are less complex. wont return anything if length between any vertice combination is smaller than minDist
std::vector<Vec2> SimplifyPolygon(std::vector<Vec2> vertices, float minDist) {
    std::vector<Vec2> newVertices;
    //always add first point of vertices to new vertices.
    newVertices.push_back(vertices[0]);
    int i = 1;
    int next = i+1;
    while (i < vertices.size() - 1) {
        if (next == vertices.size() - 1) {
            next = 0;
        }
        if (minDist < std::abs((vertices[i] - vertices[next]).Magnitude())) {
            newVertices.push_back(vertices[i]);
            i = next;
        }
        if (next == 0) {
            i = vertices.size();
        }
        next++;

    }
    //always force last items to new vertices.
    newVertices.push_back(vertices[vertices.size() - 1]);

    return newVertices;
}

std::vector<std::vector<Vec2>> BreakUpPolygon(std::vector<Vec2> polygon, float unitMulti) {
    std::vector<std::vector<Vec2>> polygons;
    Vec2 startPos = polygon[0];
    bool howClose = false;
    std::vector<Vec2> currentLine;
    for (int i = 1; i < polygon.size(); i++) {
        currentLine.push_back(polygon[i]);
    }
    while (!howClose)
    {
        std::vector<Vec2> nextLine;
        for (int i = 0; i < currentLine.size(); i++)
        {
            //creates new point
            Vec2 directionVector;
            directionVector = (startPos - currentLine[i]).UnitVector();
            Vec2 newPoint = currentLine[i] + directionVector * unitMulti;

            //add new point to next line
            nextLine.push_back(newPoint);
            //thats it?
        }
        //
        //iterate through a, b, c, d, and then, c d e f like that.
        //current line and next line should be the same in length, so whatever the combination both combined will be divisible by two.
        Vec2 a, b;
        Vec2 c, d;
        a = currentLine[0];
        b = currentLine[1];
        c = nextLine[0];
        d = nextLine[1];
        int polygonCountBefore = polygons.size();
        bool distanceCheck = false;
        for (int i = 1; i < currentLine.size(); i++) {
            if (i + 1 == currentLine.size()) {
                continue;
            }
            float distToStartC = (c - startPos).Magnitude();
            float distToStartD = (d - startPos).Magnitude();
            //Only add polygons if distance is greater than unit multi * 2, c and d should always be the closer point to the start
            if (!distanceCheck && (distToStartC > unitMulti * 2 || distToStartD > unitMulti * 2)) {
                distanceCheck = true;
            }
            if (distanceCheck)//should only need to do this test once per line?
            {
                std::vector<Vec2> newPolygon;
                newPolygon.push_back(a);
                newPolygon.push_back(b);
                newPolygon.push_back(d);
                newPolygon.push_back(c);
                polygons.push_back(newPolygon);
            }
            a = currentLine[i];
            b = currentLine[i+1];
            c = nextLine[i];
            d = nextLine[i+1];
        }
        int polygonCountAfter = polygons.size();
        if (polygonCountBefore == polygonCountAfter) {
            //no polygons are being added, end loop
            //connect next line to start point
            currentLine.push_back(startPos);
            polygons.push_back(currentLine);
            howClose = true;
            continue;
        }
        currentLine = nextLine; //save nextLine after storing polygons
    }
    return polygons;
}

void LightSource::FillRays() {
    for (auto rays : currentRays) {
        std::vector<Vec2> vertices;
        vertices.push_back(rays[0]->position);
        for (int i = 0; i < rays.size(); i++) {
            vertices.push_back(rays[i]->endPos);
        }
        vertices = SimplifyPolygon(vertices, 30);
        std::vector<std::vector<Vec2>>polygons = BreakUpPolygon(vertices, 30);
        for (auto polygon : polygons) {
            Graphics::DrawFillPolygon(polygon[0].x, polygon[0].y, polygon, color);
        }
        rays.clear();//maybewillcrash
    }
    //TODO: clear currentRays somewhere else. should be cleared everyframe in a lightsource.update function.    
    currentRays.clear();
}