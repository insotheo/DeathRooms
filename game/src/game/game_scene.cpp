#include "game/scenes.h"

#include "imgui.h"
#include <format>

#include "game/map.h"

void GameScene::begin(){
    if(!wall_texture.loadFromFile("./assets/brick.png")){return;}
    wall_sprite.setTexture(wall_texture);
    wall_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(wall_texture.getSize().x), static_cast<int>(wall_texture.getSize().y)}));
    wall_sprite.setScale({2.f, 2.f});
    wall_sprite.setColor(sf::Color(100, 130, 250));
    wall_sprite.setOrigin(wall_sprite.getLocalBounds().getCenter());

    if(!floor_texture.loadFromFile("./assets/floor.png")){return;}
    floor_sprite.setTexture(floor_texture);
    floor_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(floor_texture.getSize().x), static_cast<int>(floor_texture.getSize().y)}));
    floor_sprite.setScale({2.f, 2.f});
    floor_sprite.setColor(sf::Color(200, 200, 200));
    floor_sprite.setOrigin(floor_sprite.getLocalBounds().getCenter());

    gen_map(50, wall_sprite, walls, floor_sprite, floors, {{5, 7}, {7, 5}, {4, 8}});

    m_cam.setSize(sf::Vector2f(960.f, 540.f)); //16:9
    p_clear_color = sf::Color(35,35,35);

    m_player = new Player({0, 0});
}

void GameScene::tick(TICK_ARGS){
    if(m_dbg_mode){
        m_fps = static_cast<int>((1.f / dt) * 10) / 10.f; 
    }

    m_player->call_tick(_TICK_ARGS, *this);
    m_cam.setCenter(m_player->get_rect().getCenter());
    wnd.setView(m_cam);
}

void GameScene::render(RENDER_ARGS){
    const sf::FloatRect view_rect(m_cam.getCenter() - (m_cam.getSize() / 2.f), m_cam.getSize() * 1.25f); 

    for(const auto& w : walls){
        if(view_rect.findIntersection(w).has_value()){
            wall_sprite.setPosition(w.position);
            wnd.draw(wall_sprite);
        }
    }

    for(const auto& f : floors){
        if(view_rect.findIntersection(f).has_value()){
            floor_sprite.setPosition(f.position);
            wnd.draw(floor_sprite);
        }
    }

    m_player->call_render(_RENDER_ARGS);

    if(m_dbg_mode){
        auto [x, y] = m_player->get_pos_pair();
        ImGui::Begin("DBG");
        ImGui::Text(std::format("FPS: {}", m_fps).c_str());
        ImGui::Text(std::format("Player pos: x = {} ; y = {}", static_cast<int>(x * 10) / 10.f, static_cast<int>(y * 10) / 10.f).c_str());
        ImGui::End();
    }
}

void GameScene::event(EVENT_ARGS){
    if(const auto& key_rel_e = e->getIf<sf::Event::KeyReleased>()){
        if(key_rel_e->code == sf::Keyboard::Key::F2){
            m_dbg_mode = !m_dbg_mode;
        }
    }
}

void GameScene::finish(){
    walls.clear();
    floors.clear();

    wall_texture = {};
    floor_texture = {};

    if(m_player){
        delete m_player;
    }
}