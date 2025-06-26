#ifndef ENTITY_H
#define ENTITY_H

#include "engine/scene.h"
#include "engine/args.h"

class Entity{
public:
    Entity() {}
    ~Entity() = default;

    inline void call_begin()                                {begin();}; 
    inline void call_tick(ENTITY_TICK_ARGS)                 {tick(_TICK_ARGS, scene);}; 
    inline void call_render(RENDER_ARGS)                    {render(_RENDER_ARGS);}; 
    inline void call_event(EVENT_ARGS)                      {event(_EVENT_ARGS);}; 
    inline void call_finish()                               {finish();}; 
protected:
    virtual void begin() {};
    virtual void tick(ENTITY_TICK_ARGS) {};
    virtual void render(RENDER_ARGS) {};
    virtual void event(EVENT_ARGS) {};
    virtual void finish() {};
};

#define _ENTITY public Entity

#endif