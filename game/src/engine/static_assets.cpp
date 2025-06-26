#include "static_assets.h"

#define FAILED_TEXTURE_LOADING std::cerr << "Failed to load a texture\n"

sf::Texture StaticAssets::null_texture;

void StaticAssets::load(){
    if(!null_texture.loadFromFile(path("null_texture.png"))){
        FAILED_TEXTURE_LOADING;
    }
}