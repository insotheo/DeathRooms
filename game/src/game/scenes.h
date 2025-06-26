#ifndef SCENES_H
#define SCENES_H

#include <iostream>
#include "engine/scene.h"
#include "game/entities.h"

class GameScene : _SCENE{
protected:
    void begin() override;
    void tick(TICK_ARGS) override;
    void render(RENDER_ARGS) override;
    void finish() override;

private:
    Player* m_player;
};

#endif