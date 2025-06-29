#include "game/entities.h"

bool Player::s_loaded = false;
sf::Texture Player::s_idle_texture;
sf::Texture Player::s_run_texture;

void Player::tick(ENTITY_TICK_ARGS){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
        m_dir.y -= 1.f;
        m_is_moving = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
        m_dir.y += 1.f;
        m_is_moving = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        m_dir.x -= 1.f;
        m_looking_right = false;
        m_is_moving = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        m_dir.x += 1.f;
        m_looking_right = true;
        m_is_moving = true;
    }

    //rotation
    {
        const sf::Vector2f init = m_sprite.getScale();
        if((m_looking_right && init.x < 0) || (!m_looking_right && init.x > 0)){
            m_sprite.setScale({-init.x, init.y});
        }
    }

    //position update
    if(m_is_moving){
        float len = std::sqrtf(m_dir.x * m_dir.x + m_dir.y * m_dir.y);
        if(len != 0.f){
            m_dir /= len;
            m_dir *= m_speed * dt;
            m_pos += m_dir;
            m_dir = {0.f, 0.f};
        }
        m_animator.set_anim("RUN");
    }
    else{
        m_animator.set_anim("IDLE");
    }
    
    m_sprite.setPosition(m_pos);
    m_animator.tick(dt);
    m_is_moving = false;

    p_entity_rect = m_sprite.getGlobalBounds();
}

void Player::render(RENDER_ARGS){
    wnd.draw(m_sprite);
}

void Player::finish(){
    if(s_loaded){
        s_idle_texture = {};
    }
}