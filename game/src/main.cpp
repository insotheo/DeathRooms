#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Death Rooms");
    window.setVerticalSyncEnabled(true);
    sf::Clock timer;

    timer.restart();
    while(window.isOpen()){
        while(const auto& e = window.pollEvent()){
            if(e->is<sf::Event::Closed>()){
                timer.reset();
                window.close();
            }
        }
        sf::Time delta_time = timer.restart();

        window.clear(sf::Color::White);

        window.display();
    }

    return 0;
}