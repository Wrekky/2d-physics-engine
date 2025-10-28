#include "Application.h"

int main(int argc, char *args[]) {
    //TODO: Multiple scenes
    //If scene 1:
    Application app;
    app.Setup();
    while (app.IsRunning()) {
        app.Input();
        app.Update();
        app.Render();
    }
    app.Destroy();
    //If scene 2:

    

    return 0;
}