#ifndef ALAMO_ENGINE_H
#define ALAMO_ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Layout.h"

enum class ScreenState {
    MENU,
    GAME
};

class Engine {
public:
    void run();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    bool running = true;
    int selectedMenuIndex = 0;

    Layout layout;
    ScreenState currentScreen = ScreenState::MENU;

    void input();
    void update();
    void render();

    void renderMenu();
    void renderGame();
    void drawText(const char* text, int x, int y, SDL_Color color);
};

#endif