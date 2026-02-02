#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <iostream>
#include <string>

#include "../Graphics.h"
#include "../Physics/World.h"
#include "../Text.h"
#include "../Physics/Constants.h"
#include "../UI/UIObject.h"

class Scene {
    private:
    public:
        World* world;
        std::vector<UIObject*> UIObjects;
        std::vector<Text*> textObjects;
        TTF_Font* defaultFont;
        
        int nextScene = 0;
        bool switchScene = false;
        bool running = false;
        bool debug = false;
        bool IsRunning();
        void FontSetup();
        virtual void Setup();
        virtual void Input();
        virtual void Update();

        Scene() = default;
        ~Scene() = default;
};

//TODO make overrides for different scenes
class SceneOne: public Scene {
    private:
    public:
        void Setup() override;
        void Input() override;
        void Update() override;
}; 

//Probably bad to do stuff like this but i'm messing around.
class TitleScreen: public Scene {
    private:
    public:
        void Setup() override;
        void Input() override;
        void Update() override;
};
#endif