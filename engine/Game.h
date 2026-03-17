#ifndef ALAMO_GAME_H
#define ALAMO_GAME_H

class Game {
public:
    virtual void input() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual ~Game() = default;
};

#endif