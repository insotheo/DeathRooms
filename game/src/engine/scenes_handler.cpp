#include "engine/scenes_handler.h"

std::unordered_map<std::string, Scene*> ScenesHandler::s_scenes;
std::string ScenesHandler::s_current_scene = NULL_SCENE;

void ScenesHandler::go_scene(const std::string& scene_name){
    if(s_current_scene == scene_name){
        return;
    }
    const auto it = s_scenes.find(scene_name);
    if(it == s_scenes.end()){
        std::cerr << "Scene " << scene_name << " not found!\n";
        return; 
    }
    if(s_current_scene != NULL_SCENE){
        s_scenes[s_current_scene]->call_finish();
    }
    s_current_scene = scene_name;
    it->second->call_begin();
}