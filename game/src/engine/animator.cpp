#include "engine/animator.h"

void Animator::tick(float dt){
    if(m_current_anim == NULL_ANIM){
        return;
    }

    const Animation& anim = m_animations.at(m_current_anim);

    if (frame + 1 >= static_cast<size_t>(anim.frames)) {
        if (anim.is_loop) {
            frame = 0;
        } else {
            return;
        }
    } else {
        frame += 1;
    }
    m_timer += dt;

    if(m_timer >= anim.delay){
        m_sprite.setTextureRect(sf::IntRect(
            sf::Vector2i(static_cast<int>(frame * anim.frame_w), 0),
            sf::Vector2i(anim.frame_w, anim.frame_h)
        ));
        m_timer = 0.f;
    }
}