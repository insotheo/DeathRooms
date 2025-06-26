#ifndef STATIC_ASSETS_H
#define STATIC_ASSETS_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class StaticAssets{
public:
    static void load();

    static sf::Texture null_texture;
private:
    static inline std::string path(const std::string& p){
        return "./assets/" + p;
    }
};

#endif