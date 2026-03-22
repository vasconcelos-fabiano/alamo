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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "Erro SDL_image: " << IMG_GetError() << std::endl;
    }

    SDL_Surface *surface = IMG_Load("assets/logo-alamo-arcade.png");
    if (!surface)
    {
        std::cout << "Erro ao carregar logo: " << IMG_GetError() << std::endl;
    }
    else
    {
        logoTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
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
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_f:
            {
                Uint32 flags = SDL_GetWindowFlags(window);

                if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
                    SDL_SetWindowFullscreen(window, 0);
                else
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

                int width = 0;
                int height = 0;
                SDL_GetRendererOutputSize(renderer, &width, &height);
                layout = createLayout(width, height);
                break;
            }

            case SDLK_ESCAPE:
                running = false;
                break;

            case SDLK_UP:
                upPressed = true;

                if (currentScreen == ScreenState::MENU && selectedMenuIndex > 0)
                    selectedMenuIndex--;

                break;

            case SDLK_DOWN:
                downPressed = true;

                if (currentScreen == ScreenState::MENU && selectedMenuIndex < 2)
                    selectedMenuIndex++;

                break;

            case SDLK_LEFT:
                leftPressed = true;

                if (currentScreen == ScreenState::MENU && selectedMenuIndex < 2)
                    selectedMenuIndex++;

                break;

            case SDLK_RIGHT:
                rightPressed = true;

                if (currentScreen == ScreenState::MENU && selectedMenuIndex < 2)
                    selectedMenuIndex++;

                break;

            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                if (currentScreen == ScreenState::MENU)
                {
                    switch (selectedMenuIndex)
                    {
                    case 0:
                        currentScreen = ScreenState::GAME;
                        break;
                    case 1:
                        currentScreen = ScreenState::INPUT_TEST;
                        break;
                    case 2:
                        running = false;
                        break;
                    }
                }
                break;

            case SDLK_BACKSPACE:
                if (currentScreen == ScreenState::GAME)
                    currentScreen = ScreenState::MENU;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                upPressed = false;
                break;

            case SDLK_DOWN:
                downPressed = false;
                break;

            case SDLK_LEFT:
                leftPressed = false;
                break;

            case SDLK_RIGHT:
                rightPressed = false;
                break;
            }
            break;

        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                layout = createLayout(event.window.data1, event.window.data2);
            break;
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

    if (logoTexture)
    {
        int logoW, logoH;
        SDL_QueryTexture(logoTexture, NULL, NULL, &logoW, &logoH);

        int targetW = 360;
        int targetH = (logoH * targetW) / logoW;

        int x = layout.gameViewport.x + (layout.gameViewport.w - targetW) / 2;
        int y = layout.gameViewport.y + 40;

        SDL_Rect dst = {x, y, targetW, targetH};
        SDL_RenderCopy(renderer, logoTexture, NULL, &dst);
    }

    if (currentScreen == ScreenState::MENU)
    {
        renderMenu();
    }
    else if (currentScreen == ScreenState::GAME)
    {
        renderGame();
    }

    else if (currentScreen == ScreenState::INPUT_TEST)
    {
        renderInputTest();
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
    int item1Y = centerY;
    int item2Y = centerY + 50;
    int item3Y = centerY + 100;

    // centralizar aproximado (ajuste fino depois)
    int textOffset = 100;
    int cursorOffset = 140;

    int cursorY;

    if (selectedMenuIndex == 0)
        cursorY = item1Y;
    else if (selectedMenuIndex == 1)
        cursorY = item2Y;
    else
        cursorY = item3Y;

    drawText(">", centerX - cursorOffset, cursorY, selectedColor);

    // drawText("Alamo Arcade", centerX - textOffset, titleY, selectedColor);
    drawText("Game List", centerX - textOffset, item1Y, selectedMenuIndex == 0 ? selectedColor : normalColor);
    drawText("Input Test", centerX - textOffset, item2Y, selectedMenuIndex == 1 ? selectedColor : normalColor);
    drawText("Exit", centerX - textOffset, item3Y, selectedMenuIndex == 2 ? selectedColor : normalColor);
}

void Engine::renderGame()
{
    SDL_Color color = {220, 220, 220, 255};

    int x = layout.gameViewport.x + 80;
    int y = layout.gameViewport.y + 200;

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

void Engine::renderInputTest()
{
    int baseX = layout.gameViewport.x;
    int baseY = layout.gameViewport.y;
    int w = layout.gameViewport.w;
    int h = layout.gameViewport.h;

    // === DIRECIONAL (ESQUERDA) ===
    int dpadX = baseX + 50;
    int dpadY = baseY + h - 200;

    SDL_Rect up = {dpadX + 40, dpadY, 40, 40};
    SDL_Rect down = {dpadX + 40, dpadY + 80, 40, 40};
    SDL_Rect left = {dpadX, dpadY + 40, 40, 40};
    SDL_Rect right = {dpadX + 80, dpadY + 40, 40, 40};

    // UP
    if (upPressed)
        SDL_SetRenderDrawColor(renderer, 255, 220, 120, 255);
    else
        SDL_SetRenderDrawColor(renderer, 80, 80, 100, 255);

    SDL_RenderFillRect(renderer, &up);

    // DOWN
    if (downPressed)
        SDL_SetRenderDrawColor(renderer, 255, 220, 120, 255);
    else
        SDL_SetRenderDrawColor(renderer, 80, 80, 100, 255);

    SDL_RenderFillRect(renderer, &down);

    // LEFT
    if (leftPressed)
        SDL_SetRenderDrawColor(renderer, 255, 220, 120, 255);
    else
        SDL_SetRenderDrawColor(renderer, 80, 80, 100, 255);

    SDL_RenderFillRect(renderer, &left);

    // RIGHT
    if (rightPressed)
        SDL_SetRenderDrawColor(renderer, 255, 220, 120, 255);
    else
        SDL_SetRenderDrawColor(renderer, 80, 80, 100, 255);

    SDL_RenderFillRect(renderer, &right);

    // === BOTÕES ABC (DIREITA) ===
    int btnX = baseX + w - 150;
    int btnY = baseY + h - 200;

    SDL_Rect A = {btnX, btnY, 50, 50};
    SDL_Rect B = {btnX - 60, btnY + 60, 50, 50};
    SDL_Rect C = {btnX + 60, btnY + 60, 50, 50};

    SDL_SetRenderDrawColor(renderer, 80, 80, 100, 255);
    SDL_RenderFillRect(renderer, &A);
    SDL_RenderFillRect(renderer, &B);
    SDL_RenderFillRect(renderer, &C);

    // === START / SAVE ===
    SDL_Rect start = {baseX + w - 180, baseY + 50, 60, 30};
    SDL_Rect save = {baseX + w - 100, baseY + 50, 60, 30};

    SDL_SetRenderDrawColor(renderer, 80, 80, 100, 255);
    SDL_RenderFillRect(renderer, &start);
    SDL_RenderFillRect(renderer, &save);

    // === RESET ===
    SDL_Rect reset = {baseX + 50, baseY + 50, 80, 30};

    SDL_RenderFillRect(renderer, &reset);

    // === TÍTULO ===
    drawText("INPUT TEST", baseX + 50, baseY + 20, {255, 255, 255, 255});
}