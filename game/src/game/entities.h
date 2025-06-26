#ifndef ENTITIES_H
#define ENTITIES_H

#include <SFML/Graphics.hpp>
#include "engine/entity.h"
#include "engine/animator.h"
#include "engine/static_assets.h"

class Player : _ENTITY{
public:
    Player()
    : m_sprite(StaticAssets::null_texture), m_animator(m_sprite), m_pos(100.f, 100.f)
    {
        m_sprite.setScale({2.5f, 2.5f});

        if(!s_loaded){
            s_loaded = true;
            if(!s_idle_texture.loadFromFile("./assets/player/idle.png")){s_loaded = false;}
        }

        m_animator.push_anim("IDLE", Animation(&s_idle_texture, 11, 20, 32, 32, true));

        m_animator.set_anim("IDLE");
    }

    ~Player(){
        finish();
    }
protected:
    void tick(ENTITY_TICK_ARGS) override; 
    void render(RENDER_ARGS) override;
    void finish() override;

private:
    sf::Sprite m_sprite;
    Animator m_animator;

    sf::Vector2f m_pos;

    static bool s_loaded;
    static sf::Texture s_idle_texture;
};

#endif