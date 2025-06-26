#include "game/entities.h"

bool Player::s_loaded = false;
sf::Texture Player::s_idle_texture;

void Player::tick(ENTITY_TICK_ARGS){
    m_animator.tick(dt);

    m_sprite.setPosition(m_pos);
}

void Player::render(RENDER_ARGS){
    wnd.draw(m_sprite);
}

void Player::finish(){
    if(s_loaded){
        s_idle_texture = {};
    }
}