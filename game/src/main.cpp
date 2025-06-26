#include <SFML/Graphics.hpp>

#include "engine/scenes_handler.h"
#include "game/scenes.h"

int main(){
    //scenes defs
    GameScene* game_scene = NEW_SCENE(GameScene);

    ScenesHandler::push_scene("GAME_SCENE", game_scene);

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Death Rooms");
    window.setVerticalSyncEnabled(true);
    sf::Clock timer;

    timer.restart();
    GO_SCENE("GAME_SCENE");
    while(window.isOpen()){
        while(const auto& e = window.pollEvent()){
            if(e->is<sf::Event::Closed>()){
                timer.reset();
                window.close();
            }
            ScenesHandler::call_event(e, window);
        }
        sf::Time delta_time = timer.restart();
        ScenesHandler::call_tick(delta_time.asSeconds(), window);

        window.clear(ScenesHandler::get_clear_color());
        ScenesHandler::call_render(window);
        window.display();
    }
    ScenesHandler::finish();

    return 0;
}