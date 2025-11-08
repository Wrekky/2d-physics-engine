#include "Application.h"

int main(int argc, char *args[]) {
    //TODO: Multiple scenes
    Application app;
    app.scene = new SceneOne();
    app.scene->Setup();
    while (app.scene->IsRunning()) {
        app.scene->Input();
        app.scene->Update();
        app.Render();
    }
    app.Destroy();

    return 0;
}