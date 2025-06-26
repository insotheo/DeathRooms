#ifndef SCENES_HANDLER_H
#define SCENES_HANDLER_H

#include <iostream>
#include <unordered_map>
#include <string>
#include "engine/scene.h"

#define NULL_SCENE "NULL"

class ScenesHandler{
public:
    static void go_scene(const std::string& scene_name);
    static inline void push_scene(const std::string& scene_name, Scene* scene_p){
        s_scenes[scene_name] = scene_p;
    }
    static inline void finish(){
        for(const auto& [name, scene] : s_scenes){
            delete scene;
        }
        s_scenes.clear();
    }

    static inline void call_tick(TICK_ARGS){
        if(s_current_scene == NULL_SCENE){
            return;
        }
        s_scenes[s_current_scene]->call_tick(_TICK_ARGS);
    }

    static inline void call_render(RENDER_ARGS){
        if(s_current_scene == NULL_SCENE){
            return;
        }
        s_scenes[s_current_scene]->call_render(_RENDER_ARGS);
    }

    static inline void call_event(EVENT_ARGS){
        if(s_current_scene == NULL_SCENE){
            return;
        }
        s_scenes[s_current_scene]->call_event(_EVENT_ARGS);
    }
    
    static inline const sf::Color& get_clear_color() {
        if(s_current_scene == NULL_SCENE){
            return sf::Color::Black;
        }
        return s_scenes[s_current_scene]->get_clear_color();
    }

private:
    static std::unordered_map<std::string, Scene*> s_scenes;
    static std::string s_current_scene;
};

#define NEW_SCENE(class_name) new class_name()
#define GO_SCENE(scene_name) ScenesHandler::go_scene(scene_name)

#endif