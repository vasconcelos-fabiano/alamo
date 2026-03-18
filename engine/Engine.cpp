#include "Engine.h"
#include <iostream>

void Engine::run()
{
    std::cout << "Alamo Arcade Engine started." << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        return;
    }

    if (TTF_Init() != 0)
    {
        std::cout << "Erro ao inicializar SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    window = SDL_CreateWindow(
        "Alamo Arcade",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1400, 720,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        std::cout << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        std::cout << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    font = TTF_OpenFont("/System/Library/Fonts/SFNSMono.ttf", 28);

    if (!font)
    {
        std::cout << "Erro ao carregar fonte: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    int width = 0;
    int height = 0;
    SDL_GetRendererOutputSize(renderer, &width, &height);
    layout = createLayout(width, height);

    while (running)
    {
        input();
        update();
        render();
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Engine::input()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }

            if (currentScreen == ScreenState::MENU)
            {
                if (event.key.keysym.sym == SDLK_UP)
                {
                    if (selectedMenuIndex > 0)
                    {
                        selectedMenuIndex--;
                    }
                }

                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    if (selectedMenuIndex < 1)
                    {
                        selectedMenuIndex++;
                    }
                }

                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
                {
                    if (selectedMenuIndex == 0)
                    {
                        currentScreen = ScreenState::GAME;
                    }
                    else if (selectedMenuIndex == 1)
                    {
                        running = false;
                    }
                }
            }
            else if (currentScreen == ScreenState::GAME)
            {
                if (event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    currentScreen = ScreenState::MENU;
                }
            }
        }

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            layout = createLayout(event.window.data1, event.window.data2);
        }
    }
}

void Engine::update()
{
}

void Engine::render()
{
    SDL_SetRenderDrawColor(renderer, 12, 12, 18, 255);
    SDL_RenderClear(renderer);

    SDL_Rect left = {layout.leftPanel.x, layout.leftPanel.y, layout.leftPanel.w, layout.leftPanel.h};
    SDL_Rect center = {layout.gameViewport.x, layout.gameViewport.y, layout.gameViewport.w, layout.gameViewport.h};
    SDL_Rect right = {layout.rightPanel.x, layout.rightPanel.y, layout.rightPanel.w, layout.rightPanel.h};

    SDL_SetRenderDrawColor(renderer, 28, 28, 38, 255);
    SDL_RenderFillRect(renderer, &left);
    SDL_RenderFillRect(renderer, &right);

    SDL_SetRenderDrawColor(renderer, 0, 0, 90, 255);
    SDL_RenderFillRect(renderer, &center);

    SDL_SetRenderDrawColor(renderer, 170, 170, 190, 255);
    SDL_RenderDrawRect(renderer, &center);

    SDL_Rect innerBorder = {
        center.x + 2,
        center.y + 2,
        center.w - 4,
        center.h - 4};
    SDL_SetRenderDrawColor(renderer, 110, 110, 140, 255);
    SDL_RenderDrawRect(renderer, &innerBorder);

    if (currentScreen == ScreenState::MENU)
    {
        renderMenu();
    }
    else if (currentScreen == ScreenState::GAME)
    {
        renderGame();
    }

    SDL_RenderPresent(renderer);
}

void Engine::renderMenu()
{
    SDL_Color normalColor = {200, 200, 210, 255};
    SDL_Color selectedColor = {255, 220, 120, 255};

    int centerX = layout.gameViewport.x + layout.gameViewport.w / 2;
    int centerY = layout.gameViewport.y + layout.gameViewport.h / 2;

    // offsets verticais do menu
    int titleY = layout.gameViewport.y + 60;
    int item1Y = centerY;
    int item2Y = centerY + 50;

    // centralizar aproximado (ajuste fino depois)
    int textOffset = 100;
    int cursorOffset = 140;

    int cursorY = (selectedMenuIndex == 0) ? item1Y : item2Y;

    drawText(">", centerX - cursorOffset, cursorY, selectedColor);

    drawText("Alamo Arcade", centerX - textOffset, titleY, selectedColor);
    drawText("Game List", centerX - textOffset, item1Y, selectedMenuIndex == 0 ? selectedColor : normalColor);
    drawText("Exit", centerX - textOffset, item2Y, selectedMenuIndex == 1 ? selectedColor : normalColor);
}

    void Engine::renderGame()
    {
        SDL_Color color = {220, 220, 220, 255};

        int x = layout.gameViewport.x + 80;
        int y = layout.gameViewport.y + 80;

        drawText("GAME SCREEN", x, y, color);
        drawText("Press BACKSPACE to return", x, y + 60, color);
    }

    void Engine::drawText(const char *text, int x, int y, SDL_Color color)
    {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
        if (!surface)
        {
            return;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture)
        {
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect dst = {x, y, surface->w, surface->h};

        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }