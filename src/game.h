#ifndef GAME_H
#define GAME_H
#include "SFML/Graphics.hpp"
#include "constants.h"
class Game : public sf::Drawable {
    private:
    std::vector<std::vector<bool>> cells;
    sf::VertexArray vertices;
    int getNeighbourCount(std::vector<std::vector<bool>>& grid, int x, int y);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(this->vertices);
    }
    public:
    void update();
    void set(int x, int y, bool active);
    bool get(int x, int y);
    void updateVertexArray();
    
    Game();
};
#endif