#include "game/scenes.h"

void GameScene::begin(){
    p_clear_color = sf::Color(35,35,35);

    m_player = new Player();
}

void GameScene::tick(TICK_ARGS){
    m_player->call_tick(_TICK_ARGS, *this);
}

void GameScene::render(RENDER_ARGS){
    m_player->call_render(_RENDER_ARGS);
}

void GameScene::finish(){
    if(m_player){
        delete m_player;
    }
}