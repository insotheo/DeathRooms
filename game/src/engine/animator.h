#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

#define NULL_ANIM "NULL"

class Animation
{
public:
    Animation(sf::Texture* texture, int frames_count, int fps, int h, int w, bool loop = false)
        : texture_map(texture), frames(frames_count), delay(fps == 0 ? 0.f : 1.f/static_cast<float>(fps)), frame_h(h), frame_w(w), is_loop(loop)
    {}
    Animation() = default;

    sf::Texture* texture_map = nullptr;
    int frames = 0;
    int frame_h = 0;
    int frame_w = 0;
    float delay = 0;
    bool is_loop = false;
};

class Animator{
public:
    Animator(sf::Sprite& sprite)
    : m_sprite(sprite), m_current_anim(NULL_ANIM), m_timer(0.f)
    {}
    ~Animator() = default;

    void tick(float dt);

    inline void push_anim(const std::string& anim_name, const Animation& anim){
        m_animations[anim_name] = anim;
    }

    inline void set_anim(const std::string& anim_name){
        if(m_current_anim == anim_name){
            return;
        }
        
        const auto it = m_animations.find(anim_name);
        if(it == m_animations.end()){
            return;
        }
        m_current_anim = anim_name;
        frame = 0;
        m_timer = 0.f;

        const Animation& anim = m_animations.at(m_current_anim);
        m_sprite.setTexture(*anim.texture_map);
        m_sprite.setTextureRect(sf::IntRect({0, 0}, sf::Vector2(anim.frame_w, anim.frame_h)));
        m_sprite.setOrigin(m_sprite.getLocalBounds().getCenter());
    }

    inline const std::string& get_current_anim() const { return m_current_anim; }
private:
    sf::Sprite& m_sprite;
    size_t frame;
    std::string m_current_anim;
    std::unordered_map<std::string, Animation> m_animations;
    float m_timer;
};

#endif