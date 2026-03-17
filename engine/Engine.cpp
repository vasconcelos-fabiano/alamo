#include "Engine.h"
#include <iostream>
#include <thread>
#include <chrono>

void Engine::run() {

    std::cout << "Alamo Engine started." << std::endl;

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