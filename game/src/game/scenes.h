#ifndef SCENES_H
#define SCENES_H

#include <iostream>
#include <vector>
#include "engine/scene.h"
#include "engine/static_assets.h"
#include "game/entities.h"

class GameScene : _SCENE{
public:
    GameScene()
    : wall_sprite(StaticAssets::null_texture), floor_sprite(StaticAssets::null_texture), m_dbg_mode(false), m_fps(0.f)
    {}

protected:
    void begin() override;
    void tick(TICK_ARGS) override;
    void render(RENDER_ARGS) override;
    void event(EVENT_ARGS) override;
    void finish() override;

private:
    bool m_dbg_mode;
    float m_fps;

    sf::View m_cam;
    Player* m_player;

    sf::Texture wall_texture;
    sf::Texture floor_texture;
    sf::Sprite wall_sprite;
    sf::Sprite floor_sprite;
    std::vector<sf::FloatRect> walls;
    std::vector<sf::FloatRect> floors;
};

#endif