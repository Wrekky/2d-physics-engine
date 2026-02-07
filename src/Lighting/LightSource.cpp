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