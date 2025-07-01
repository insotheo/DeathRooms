#ifndef ENTITIES_H
#define ENTITIES_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <utility>
#include "engine/entity.h"
#include "engine/animator.h"
#include "engine/static_assets.h"

class Player : _ENTITY{
public:
    Player(const sf::Vector2f init_pos)
    : m_speed(150.f), 
    m_sprite(StaticAssets::null_texture), m_animator(m_sprite), m_pos(init_pos), m_looking_right(true), m_dir(0.f, 0.f), m_is_moving(false), m_attacking(false)
    {

        if(!s_loaded){
            s_loaded = true;

            if(!s_idle_texture.loadFromFile("./assets/player/idle.png")){s_loaded = false;}
            if(!s_run_texture.loadFromFile("./assets/player/run.png")){s_loaded = false;}
            if(!s_attack1_texture.loadFromFile("./assets/player/attack1.png")){s_loaded = false;}
            if(!s_attack2_texture.loadFromFile("./assets/player/attack2.png")){s_loaded = false;}
        }

        m_animator.push_anim("IDLE", Animation(&s_idle_texture, 8, 10, 192, 192, true, false));
        m_animator.push_anim("RUN", Animation(&s_run_texture, 6, 14, 192, 192, true, false));
        m_animator.push_anim("ATTACK1", Animation(&s_attack1_texture, 4, 8, 192, 192, false, true));
        m_animator.push_anim("ATTACK2", Animation(&s_attack2_texture, 4, 8, 192, 192, false, true));

        m_animator.set_anim("IDLE");
    }

    ~Player(){
        finish();
    }

    inline const std::pair<float, float> get_pos_pair() const {
        return {m_pos.x, m_pos.y};
    }

    inline sf::Vector2f& get_pos() {
        return m_pos;
    }
    
    inline void update_pos(){
        m_sprite.setPosition(m_pos);
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
    bool m_attacking;

    static bool s_loaded;
    static sf::Texture s_idle_texture;
    static sf::Texture s_run_texture;
    static sf::Texture s_attack1_texture;
    static sf::Texture s_attack2_texture;
};

#endif