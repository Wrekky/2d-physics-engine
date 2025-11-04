#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/World.h"
#include <vector>
#include "./Text.h"
#include "./Scene/Scene.h"
class Application {
    private:
    public:
    //TODO: create scene on application constructor
        Scene* scene;
        Application() = default;
        ~Application() = default;
        void Render();
        void Destroy();
};

#endif