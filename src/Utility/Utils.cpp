#include "Utils.h"
float Utils::distance(Vec2 a, Vec2 b) {
    return sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}