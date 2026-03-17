#ifndef ALAMO_ENGINE_H
#define ALAMO_ENGINE_H

class Engine {
public:
    void run();

private:
    void input();
    void update();
    void render();
};

#endif