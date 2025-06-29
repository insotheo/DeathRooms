#ifndef ENTITIES_H
#define ENTITIES_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "engine/entity.h"
#include "engine/animator.h"
#include "engine/static_assets.h"

class Player : _ENTITY{
public:
    Player()
    : m_speed(100.f), 
    m_sprite(StaticAssets::null_texture), m_animator(m_sprite), m_pos(100.f, 100.f), m_looking_right(true), m_dir(0.f, 0.f), m_is_moving(false)
    {
        m_sprite.setScale({1.25f, 1.25f});

        if(!s_loaded){
            s_loaded = true;

            if(!s_idle_texture.loadFromFile("./assets/player/idle.png")){s_loaded = false;}
            if(!s_run_texture.loadFromFile("./assets/player/run.png")){s_loaded = false;}
        }

        m_animator.push_anim("IDLE", Animation(&s_idle_texture, 8, 10, 192, 192, true));
        m_animator.push_anim("RUN", Animation(&s_run_texture, 6, 14, 192, 192, true));

        m_animator.set_anim("RUN");
    }

    ~Player(){
        finish();
    }
protected:
    void tick(ENTITY_TICK_ARGS) override; 
    void render(RENDER_ARGS) override;
    void finish() override;

private:
    float m_speed;

private:
    sf::Sprite m_sprite;
    Animator m_animator;

    sf::Vector2f m_pos;
    sf::Vector2f m_dir;
    bool m_is_moving;
    bool m_looking_right;

    static bool s_loaded;
    static sf::Texture s_idle_texture;
    static sf::Texture s_run_texture;
};

#endif