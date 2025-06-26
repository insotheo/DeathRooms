#ifndef ARGS_H
#define ARGS_H

#include <SFML/Graphics.hpp>

#define TICK_ARGS float dt, sf::RenderWindow& wnd
#define ENTITY_TICK_ARGS float dt, sf::RenderWindow& wnd, Scene& scene
#define _TICK_ARGS dt, wnd

#define RENDER_ARGS sf::RenderWindow& wnd
#define _RENDER_ARGS wnd

#define EVENT_ARGS const std::optional<sf::Event>& e, sf::RenderWindow& wnd
#define _EVENT_ARGS e, wnd

#endif