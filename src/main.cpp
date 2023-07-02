#include <iostream>
#include "SFML/Graphics.hpp"
#include "game.h"
#include "constants.h"
int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Conway's Game of Life");
    sf::Clock updateGameClock;
    Game game;
    float updateInterval = 0.2;
    int w = 0;
    int a = 0;
    int s = 0;
    int d = 0;
    bool pause = false;
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
                if(event.key.code == sf::Keyboard::Z)
                    w = 1;
                if(event.key.code == sf::Keyboard::Q)
                    a = 1;
                if(event.key.code == sf::Keyboard::S)
                    s = 1;
                if(event.key.code == sf::Keyboard::D)
                    d = 1;
            }
            if(event.type == sf::Event::KeyReleased) {
                if(event.key.code == sf::Keyboard::Z)
                    w = 0;
                if(event.key.code == sf::Keyboard::Q)
                    a = 0;
                if(event.key.code == sf::Keyboard::S)
                    s = 0;
                if(event.key.code == sf::Keyboard::D)
                    d = 0;
            }
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            int x = mousePosition.x / SQUARE_SIZE - game.xDisplayOffset;
            int y = mousePosition.y / SQUARE_SIZE - game.yDisplayOffset;
            bool mouseInScreen = (mousePosition.x >= 0 && mousePosition.x < WIDTH) && (mousePosition.y >= 0 && mousePosition.y < HEIGHT);
            if(event.type == sf::Event::MouseButtonPressed && mouseInScreen)  {
                game.set(x, y, !game.get(x, y));
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseInScreen) {
                game.set(x, y, true);
            }    
        }
        if(updateGameClock.getElapsedTime().asSeconds() > updateInterval && !pause) {
            updateGameClock.restart();
            game.update();
        }
        game.xDisplayOffset += (a - d) * 0.2;
        game.yDisplayOffset += (w - s) * 0.2;
        game.updateVertexArray();
        window.clear(sf::Color::Black);
        window.draw(game);
        window.display();
    }
    return 0;
}