#ifndef RENDER_H
#define RENDER_H

#include "./Graphics.h"
#include "./Physics/World.h"
#include <vector>
#include "./Text.h"
#include "./Scene/Scene.h"
class Render {
    private:
    public:
    //TODO: create scene on Render constructor
        Scene* scene;
        SDL_Texture* texture;
        Render() = default;
        ~Render() = default;
        void RenderScene();
        void Destroy();
};

#endif