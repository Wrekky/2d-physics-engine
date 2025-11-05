//TODO: Make scene class, should inherit from application class but only override game object setup & player input. Current application.cpp will be a scene. Render should be universal.
//Will have multiple scene files defined like Scene1, Scene2, or just describing what they are.
//Will be similar to the shape class but split into multiple files due to expected size of each scene being pretty large. 
//Will have a method of switching scenes while playing, don't know the method yet.
#ifndef SCENE_H
#define SCENE_H

#include "../Graphics.h"
#include "../Physics/World.h"
#include <vector>
#include "../Text.h"


class Scene {
    private:
    public:
        World* world;
        std::vector<Text*> textObjects;
        TTF_Font* defaultFont;
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

//TODO: Menu scene, scrolling list of all scenes?
#endif