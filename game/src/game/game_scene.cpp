#include "game/scenes.h"

#include "imgui.h"
#include <format>

#include "game/map.h"

#define PLAYER_RECT_DIV 2.5f
#define WALL_RECT_DIV 1.05f

void GameScene::begin(){
    if(!wall_texture.loadFromFile("./assets/brick.png")){return;}
    wall_sprite.setTexture(wall_texture);
    wall_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(wall_texture.getSize().x), static_cast<int>(wall_texture.getSize().y)}));
    wall_sprite.setScale({2.f, 2.f});
    wall_sprite.setColor(sf::Color(100, 130, 250));

    if(!floor_texture.loadFromFile("./assets/floor.png")){return;}
    floor_sprite.setTexture(floor_texture);
    floor_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(floor_texture.getSize().x), static_cast<int>(floor_texture.getSize().y)}));
    floor_sprite.setScale({2.f, 2.f});
    floor_sprite.setColor(sf::Color(200, 200, 200));

    gen_map(25, wall_sprite, walls, floor_sprite, floors, {{5, 7}, {7, 5}, {7, 9}, {2, 2}, {9, 9}});

    m_cam.setSize(sf::Vector2f(960.f, 540.f)); //16:9
    p_clear_color = sf::Color(35, 35, 35);

    m_player = new Player({0, 0});
}

void GameScene::tick(TICK_ARGS){
    if(m_dbg_mode){
        m_fps = static_cast<int>((1.f / dt) * 10) / 10.f; 
    }

    m_player->call_tick(_TICK_ARGS, *this);
    player_collision();

    m_cam.setCenter(m_player->get_rect().getCenter());
    wnd.setView(m_cam);
}

void GameScene::render(RENDER_ARGS){
    const sf::FloatRect view_rect(m_cam.getCenter() - (m_cam.getSize() / 2.f), m_cam.getSize() * 1.25f); 

    for(const auto& w : walls){
        if(view_rect.findIntersection(w).has_value()){
            wall_sprite.setPosition(w.position);
            wnd.draw(wall_sprite);
            
            if(m_dbg_draw_collisions){
                sf::RectangleShape wall_collision(w.size / WALL_RECT_DIV);
                wall_collision.setPosition(w.getCenter() - (w.size / WALL_RECT_DIV) / 2.f);
                wall_collision.setFillColor(sf::Color::Transparent);
                wall_collision.setOutlineColor(sf::Color::Red);
                wall_collision.setOutlineThickness(2.f);
                wnd.draw(wall_collision);
            }
        }
    }

    if(m_dbg_draw_floor){
        for(const auto& f : floors){
            if(view_rect.findIntersection(f).has_value()){
                floor_sprite.setPosition(f.position);
                wnd.draw(floor_sprite);
            }
        }
    }

    m_player->call_render(_RENDER_ARGS);
    if(m_dbg_draw_collisions){
        sf::RectangleShape player_collision_shape(m_player->get_rect().size / PLAYER_RECT_DIV);
        player_collision_shape.setPosition(m_player->get_rect().position + (m_player->get_rect().size / PLAYER_RECT_DIV) / 1.5f);
        player_collision_shape.setFillColor(sf::Color::Transparent);
        player_collision_shape.setOutlineColor(sf::Color::Green);
        player_collision_shape.setOutlineThickness(2.f);

        wnd.draw(player_collision_shape);
    }

    if(m_dbg_mode){
        auto [x, y] = m_player->get_pos_pair();
        ImGui::Begin("DBG");
        ImGui::Text(std::format("FPS: {}", m_fps).c_str());
        ImGui::Text(std::format("Player pos: x = {} ; y = {}", static_cast<int>(x * 10) / 10.f, static_cast<int>(y * 10) / 10.f).c_str());
        ImGui::Checkbox("Draw collisions", &m_dbg_draw_collisions);
        ImGui::Checkbox("Draw floor", &m_dbg_draw_floor);
        ImGui::End();
    }
}

void GameScene::player_collision(){
    const sf::FloatRect valid_zone(m_cam.getCenter() - (m_cam.getSize() / 2.f), m_cam.getSize() * 0.5f);

    const sf::FloatRect player_rect(m_player->get_rect().position + (m_player->get_rect().size / PLAYER_RECT_DIV) / 1.5f, m_player->get_rect().size / PLAYER_RECT_DIV);

    for(const auto& w : walls){
        if(!valid_zone.findIntersection(w).has_value()){
            continue;
        }
        sf::FloatRect wall(w.getCenter() - (w.size / WALL_RECT_DIV) / 2.f, w.size / WALL_RECT_DIV);
        if(player_rect.findIntersection(wall).has_value()){
            float overlapLeft = player_rect.position.x + player_rect.size.x - wall.position.x;
            float overlapRight = wall.position.x + wall.size.x - player_rect.position.x;
            float overlapTop = player_rect.position.y + player_rect.size.y - wall.position.y;
            float overlapBottom = wall.position.y + wall.size.y - player_rect.position.y;

            float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

            if (minOverlap == overlapLeft) {
                //left
                m_player->get_pos().x -= overlapLeft;
            } 
            else if (minOverlap == overlapRight) {
                //right
                m_player->get_pos().x += overlapRight;
            } 
            else if (minOverlap == overlapTop) {
                //top
                m_player->get_pos().y -= overlapTop;
            } 
            else if (minOverlap == overlapBottom) {
                //bottom
                m_player->get_pos().y += overlapBottom;
            }
            m_player->update_pos();
        }
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