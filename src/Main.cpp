#include "Render.h"

int main(int argc, char *args[]) {
    //TODO: Multiple scenes
    Render app;
    app.scene = new TitleScreen();
    app.scene->running = Graphics::OpenWindow();
    app.scene->Setup();
    while (app.scene->IsRunning()) { //TODO: Move to scene class.
        if (app.scene->switchScene) {
            if (app.scene->nextScene == 0) {
                app.scene->~Scene();
                app.scene = new TitleScreen();
                app.scene->Setup();
                app.scene->running = true;
            }
            else if (app.scene->nextScene == 1) {
                app.scene->~Scene();
                app.scene = new SceneOne();
                app.scene->Setup();
                app.scene->running = true;
            }
            else if (app.scene->nextScene == 2) {
                app.scene->~Scene();
                app.scene = new Scene();
                app.scene->Setup();
                app.scene->running = true;
            }
            else if (app.scene->nextScene == 3) {
                app.scene->~Scene();
                app.scene = new TestScene();
                app.scene->Setup();
                app.scene->running = true;
            }
        }
        app.scene->Input();
        app.scene->Update();
        
        app.scene->performanceCounter = SDL_GetPerformanceCounter();
        app.RenderScene();
        app.scene->ShowFrameTime();
    }
    app.Destroy();

    return 0;
}