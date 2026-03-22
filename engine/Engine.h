#ifndef ALAMO_ENGINE_H
#define ALAMO_ENGINE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Layout.h"

enum class ScreenState
{
    MENU,
    GAME,
    INPUT_TEST
};

class Engine
{
public:
    void run();

private:
    SDL_Texture *logoTexture = nullptr;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;

    bool running = true;
    int selectedMenuIndex = 0;
    bool upPressed = false;
    bool downPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;
    bool aPressed = false;
    bool bPressed = false;
    bool cPressed = false;
    bool startPressed = false;
    bool savePressed = false;
    bool resetPressed = false;

    Layout layout;
    ScreenState currentScreen = ScreenState::MENU;

    void input();
    void update();
    void render();

    void renderMenu();
    void renderGame();
    void drawText(const char *text, int x, int y, SDL_Color color);
    void renderInputTest();
};

#endif