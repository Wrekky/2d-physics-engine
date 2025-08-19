#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Body.h"
#include "./Physics/Force.h"
#include <vector>

class Application {
    private:
        bool running = false;
        std::vector<Body*> bodys;
        std::vector<Body*> chainBodys;
        std::vector<Body*> boxBodys;
        Vec2 pushForce = Vec2(0, 0);
        
        SDL_Rect water;
        Vec2 anchor;

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