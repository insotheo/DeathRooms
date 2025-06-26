#ifndef ENTITY_H
#define ENTITY_H

#include "engine/args.h"

class Entity{
public:
    inline void call_begin()                {begin();}; 
    inline void call_tick(TICK_ARGS)        {tick(_TICK_ARGS);}; 
    inline void call_render(RENDER_ARGS)    {render(_RENDER_ARGS);}; 
    inline void call_event(EVENT_ARGS)      {event(_EVENT_ARGS);}; 
    inline void call_finish()               {finish();}; 
protected:
    virtual void begin() = 0;
    virtual void tick(TICK_ARGS) = 0;
    virtual void render(RENDER_ARGS) = 0;
    virtual void event(EVENT_ARGS) = 0;
    virtual void finish() = 0;
};

#define _ENTITY public Entity

#endif