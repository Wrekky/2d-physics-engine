#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include "./Physics/Force.h"
#include <vector>

class Application {
    private:
        bool running = false;
        std::vector<Particle*> particles;
        std::vector<Particle*> chainParticles;
        std::vector<Particle*> boxParticles;
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