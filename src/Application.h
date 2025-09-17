#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/World.h"
#include <vector>
#include "./Text.h"
class Application {
    private:
        bool running = false;
        bool debug = false;
        World* world;
        std::vector<Text*> textObjects;
        TTF_Font* roboto;
        Text* objectCountText;
    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif