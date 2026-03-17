#include "Layout.h"
#include "Engine.h"
#include <iostream>
#include <thread>
#include <chrono>

void Engine::run() {

    std::cout << "Alamo Engine started." << std::endl;

    Layout layout = createLayout(2340, 1080);

std::cout << "LeftPanel: "
          << layout.leftPanel.x << ", "
          << layout.leftPanel.y << ", "
          << layout.leftPanel.width << ", "
          << layout.leftPanel.height << std::endl;

std::cout << "GameViewport: "
          << layout.gameViewport.x << ", "
          << layout.gameViewport.y << ", "
          << layout.gameViewport.width << ", "
          << layout.gameViewport.height << std::endl;

std::cout << "RightPanel: "
          << layout.rightPanel.x << ", "
          << layout.rightPanel.y << ", "
          << layout.rightPanel.width << ", "
          << layout.rightPanel.height << std::endl;

    while (true) {

        input();
        update();
        render();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Engine::input() {
    // leitura de teclado/joystick virá depois
}

void Engine::update() {
    // lógica do jogo virá aqui
}

void Engine::render() {
    std::cout << "Frame..." << std::endl;
}
