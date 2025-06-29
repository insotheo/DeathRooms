#include "game/scenes.h"

void GameScene::begin(){
    m_cam.setSize(sf::Vector2f(960.f, 540.f)); //16:9
    p_clear_color = sf::Color(35,35,35);

    m_player = new Player();
}

void GameScene::tick(TICK_ARGS){
    m_player->call_tick(_TICK_ARGS, *this);
    m_cam.setCenter(m_player->get_rect().getCenter());
    wnd.setView(m_cam);
}

void GameScene::render(RENDER_ARGS){
    m_player->call_render(_RENDER_ARGS);
}

void GameScene::finish(){
    if(m_player){
        delete m_player;
    }
}