#include <iostream>
#include "SFML/Graphics.hpp"
#include "game.h"
#include "constants.h"
int main() {
    sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * SQUARE_SIZE, GRID_HEIGHT * SQUARE_SIZE), "Conway's Game of Life");
    sf::Clock clock;
    Game game;
    float updateInterval = 0.2;
    bool pause = false;
    game.updateVertexArray();
    window.requestFocus();
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Space) {
                    pause = !pause;
                }
                if(event.key.code == sf::Keyboard::Down) {
                    updateInterval += 0.05;
                }
                if(event.key.code == sf::Keyboard::Up && updateInterval - 0.05 > 0) {
                    updateInterval -= 0.05;
                }
            }
            if(event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                int x = mousePosition.x / SQUARE_SIZE;
                int y = mousePosition.y / SQUARE_SIZE;
                game.set(x, y, !game.get(x, y));
                game.updateVertexArray();
            }
        }
        if(clock.getElapsedTime().asSeconds() > updateInterval && !pause) {
            clock.restart();
            game.update();
            game.updateVertexArray();
        }

        window.clear(sf::Color::Black);
        window.draw(game);
        window.display();
    }
    return 0;
}