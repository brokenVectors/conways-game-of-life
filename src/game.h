#ifndef GAME_H
#define GAME_H
#include "SFML/Graphics.hpp"
#include "constants.h"
struct Cell {
    int x, y;
};

class Game : public sf::Drawable {
    private:
    //std::vector<std::vector<bool>> cells;
    std::vector<Cell> aliveCells;
    sf::VertexArray vertices;
    int getAliveNeighbourCount(std::vector<Cell>& cells, Cell cell);
    std::vector<Cell> getAllNeighbors(const Cell& cell);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(this->vertices);
    }
    public:
    float xDisplayOffset = 0;
    float yDisplayOffset = 0;
    void update();
    void set(int x, int y, bool active);
    bool get(int x, int y);
    bool cellIsAlive(const Cell& cell);
    void updateVertexArray();
    
    Game();
};
#endif