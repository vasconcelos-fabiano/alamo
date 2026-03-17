#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>

int main() {
    setenv("SDL_VIDEODRIVER", "x11", 1);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init erro: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Alamo Layout Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        0, 0,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN
        SDL_WINDOW_SHOWN
    );

    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    if (!window) {
        std::cout << "SDL_CreateWindow erro: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 2;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        std::cout << "SDL_CreateRenderer erro: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 3;
    }

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        // fundo preto
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // painel esquerdo
        SDL_Rect left = {0, 0, 150, 600};
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &left);

        // painel central
        SDL_Rect center = {150, 0, 500, 600};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &center);

        // painel direito
        SDL_Rect right = {650, 0, 150, 600};
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &right);

        // bordas
        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderDrawRect(renderer, &left);
        SDL_RenderDrawRect(renderer, &center);
        SDL_RenderDrawRect(renderer, &right);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
