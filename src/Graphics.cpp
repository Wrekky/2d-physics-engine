#include "Graphics.h"
#include <iostream>
#include <SDL2/SDL_image.h>
SDL_Window* Graphics::window = NULL;
SDL_Renderer* Graphics::renderer = NULL;
int Graphics::windowWidth = 0;
int Graphics::windowHeight = 0;

int Graphics::Width() {
    return windowWidth;
}

int Graphics::Height() {
    return windowHeight;
}

bool Graphics::OpenWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL" << std::endl;
        return false;
    }
    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    windowWidth = display_mode.w;
    windowHeight = display_mode.h;
    window = SDL_CreateWindow(NULL, 0, 0, windowWidth, windowHeight, SDL_WINDOW_FULLSCREEN);
    
    if (!window) {
        std::cerr << "Error creating SDL window" << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return false;
    }
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
    std::cout << screenSurface->format << std::endl;
    return true;
}

void Graphics::ClearScreen(Uint32 color) {
    SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);
    SDL_RenderClear(renderer);
}

void Graphics::RenderFrame() {
    SDL_RenderPresent(renderer);
}

void Graphics::DrawLine(int x0, int y0, int x1, int y1, Uint32 color) {
    lineColor(renderer, x0, y0, x1, y1, color);
}

void Graphics::DrawCircle(int x, int y, int radius, float angle, Uint32 color) {
    circleColor(renderer, x, y, radius, color);
    lineColor(renderer, x, y, x + cos(angle) * radius, y + sin(angle) * radius, color);
}

void Graphics::DrawFillCircle(int x, int y, int radius, Uint32 color) {
    filledCircleColor(renderer, x, y, radius, color);
}

void Graphics::DrawRect(int x, int y, int width, int height, Uint32 color) {
    lineColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y - height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y + height / 2.0, x - width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x - width / 2.0, y + height / 2.0, x - width / 2.0, y - height / 2.0, color);
}

void Graphics::DrawFillRect(int x, int y, int width, int height, Uint32 color) {
    boxColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
}

void Graphics::DrawPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color) {
    for (int i = 0; i < vertices.size(); i++) {
        int currIndex = i;
        int nextIndex = (i + 1) % vertices.size();
        lineColor(renderer, vertices[currIndex].x, vertices[currIndex].y, vertices[nextIndex].x, vertices[nextIndex].y, color);
    }
    filledCircleColor(renderer, x, y, 1, color);
}

void Graphics::DrawFillPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color) {
    std::vector<short> vx;
    std::vector<short> vy;
    for (int i = 0; i < vertices.size(); i++) {
        vx.push_back(static_cast<int>(vertices[i].x));
    }
    for (int i = 0; i < vertices.size(); i++) {
        vy.push_back(static_cast<int>(vertices[i].y));
    }
    filledPolygonColor(renderer, &vx[0], &vy[0], vertices.size(), color);
}

//Doesnt work for antyhing outside of the lighting system. Should be renamed.
void Graphics::DrawTexturedPolygon(int x, int y, const std::vector<Vec2>& vertices, SDL_Texture* texture, Vec2 texturePosition) {
    for (int i = 1; i < vertices.size() - 1; i++) {
        SDL_Vertex verts[3];
        verts[0].position = {vertices[0].x, vertices[0].y};
        verts[0].tex_coord = {0, 0};
        verts[0].color = {0, 0, 0, 5};

        verts[1].position = {vertices[i].x, vertices[i].y};
        verts[1].tex_coord = {i * 5, i * 5};
        verts[1].color = {0, 0, 0, 255};

        verts[2].position = {vertices[i+1].x, vertices[i+1].y};
        verts[2].tex_coord = {(i+1) * 5, (i+1) * 5};
        verts[2].color = {0, 0, 0, 255};
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);
    }
    //texturedPolygon(renderer, &vx[0], &vy[0], vertices.size(), texture, texturePosition.x, texturePosition.y);
}

void Graphics::DrawTexture(int x, int y, int width, int height, float rotation, SDL_Texture* texture) {
    SDL_Rect dstRect = {x - (width / 2), y - (height / 2), width, height};
    float rotationDeg = rotation * 57.2958;
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, rotationDeg, NULL, SDL_FLIP_NONE);
}

void Graphics::CloseWindow(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}