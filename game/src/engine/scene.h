#ifndef SCENE_H
#define SCENE_H

#include "engine/args.h"

class Scene{
public:
    Scene(){}
    ~Scene() = default;

    inline void call_begin()                {begin();}; 
    inline void call_tick(TICK_ARGS)        {tick(_TICK_ARGS);}; 
    inline void call_render(RENDER_ARGS)    {render(_RENDER_ARGS);}; 
    inline void call_event(EVENT_ARGS)      {event(_EVENT_ARGS);}; 
    inline void call_finish()               {finish();}; 

    inline const sf::Color& get_clear_color() const {return p_clear_color;}
protected:
    virtual void begin() {};
    virtual void tick(TICK_ARGS) {};
    virtual void render(RENDER_ARGS) {};
    virtual void event(EVENT_ARGS) {};
    virtual void finish() {};

    sf::Color p_clear_color = sf::Color::Black;
};

#define _SCENE public Scene

#endif