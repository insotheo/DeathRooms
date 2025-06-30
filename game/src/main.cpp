#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "engine/scenes_handler.h"
#include "engine/static_assets.h"
#include "game/scenes.h"

int main(){
    StaticAssets::load();

    //scenes defs
    GameScene* game_scene = NEW_SCENE(GameScene);

    ScenesHandler::push_scene("GAME_SCENE", game_scene);

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Death Rooms");
    window.setVerticalSyncEnabled(true);
    sf::Clock timer;

    if(!ImGui::SFML::Init(window)){
        assert(false);
    }

    timer.restart();
    GO_SCENE("GAME_SCENE");
    while(window.isOpen()){
        while(const auto& e = window.pollEvent()){
            ImGui::SFML::ProcessEvent(window, e.value());
            if(e->is<sf::Event::Closed>()){
                window.close();
            }
            ScenesHandler::call_event(e, window);
        }
        sf::Time delta_time = timer.restart();
        ImGui::SFML::Update(window, delta_time);
        ScenesHandler::call_tick(delta_time.asSeconds(), window);

        window.clear(ScenesHandler::get_clear_color());
        ScenesHandler::call_render(window);
        ImGui::SFML::Render(window);
        window.display();
    }
    ScenesHandler::finish();

    ImGui::SFML::Shutdown(window);
    return 0;
}