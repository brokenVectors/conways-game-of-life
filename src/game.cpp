#include "game.h"
#include "constants.h"
#include <array>
#include <iterator>
Game::Game() {
    for(int y = 0; y < GRID_HEIGHT; y++) {
        this->cells.push_back(std::vector<bool>());
        for(int x = 0; x < GRID_WIDTH; x++) {
            this->cells.at(y).push_back(false);
        }
    }
    this->vertices = sf::VertexArray(sf::Quads, GRID_WIDTH * GRID_HEIGHT * 4);
}
void Game::updateVertexArray() {
    // TODO: greedy meshing optimizations
    this->vertices.clear();
    this->vertices.resize(GRID_WIDTH * GRID_HEIGHT * 4);
    for(int y = 0; y < GRID_HEIGHT; y++) {
        for(int x = 0; x < GRID_WIDTH; x++) {
            this->vertices[(y*(GRID_WIDTH)+x)*4].position = sf::Vector2f(x * SQUARE_SIZE, y * SQUARE_SIZE);
            this->vertices[(y*(GRID_WIDTH)+x)*4+1].position = sf::Vector2f((x+1) * SQUARE_SIZE, y * SQUARE_SIZE);
            this->vertices[(y*(GRID_WIDTH)+x)*4+2].position = sf::Vector2f((x+1) * SQUARE_SIZE, (y+1) * SQUARE_SIZE);
            this->vertices[(y*(GRID_WIDTH)+x)*4+3].position = sf::Vector2f(x * SQUARE_SIZE, (y+1) * SQUARE_SIZE);

            sf::Color color;
            if(this->cells.at(y).at(x)) {
                color = sf::Color::White;
            }
            else {
                color = sf::Color::Black;
            }
            this->vertices[(y*(GRID_WIDTH)+x)*4].color = color;
            this->vertices[(y*(GRID_WIDTH)+x)*4+1].color = color;
            this->vertices[(y*(GRID_WIDTH)+x)*4+2].color = color;
            this->vertices[(y*(GRID_WIDTH)+x)*4+3].color = color;
        }
    }
}
void Game::update() {
    /*
        Any live cell with two or three live neighbours survives.
        Any dead cell with three live neighbours becomes a live cell.
        All other live cells die in the next generation. Similarly, all other dead cells stay dead. 
    */
   // A copy of the cells array must be made, as changes will affect other events in the same generation.
   std::vector<std::vector<bool>> cellsCopy;
   std::copy(this->cells.begin(), this->cells.end(), std::back_inserter(cellsCopy));
    for(int y = 0; y < GRID_HEIGHT; y++) {
        for(int x = 0; x < GRID_WIDTH; x++) {
            int neighbourCount = this->getNeighbourCount(cellsCopy, x, y);
            if(neighbourCount == 3) {
                // Cell gets populated
                this->cells.at(y).at(x) = true;
            }
            else if(neighbourCount != 2) {
                // Solitude / Overpopulation
                this->cells.at(y).at(x) = false;
            }
        }
    }
}
int Game::getNeighbourCount(std::vector<std::vector<bool>>& grid, int x, int y) {
    int count = 0;
    struct Coordinate {
        int x, y;
    };
    std::array<Coordinate, 8> neighbourCoords =
    {
        Coordinate {-1, -1},
        Coordinate {-1, 0},
        Coordinate {-1, 1},
        Coordinate {0, 1},
        Coordinate {0, -1},
        Coordinate {1, 1},
        Coordinate {1, 0},
        Coordinate {1, -1}
    };
    for(Coordinate coord: neighbourCoords) {
        bool inBounds = (x+coord.x >= 0 && x+coord.x < GRID_WIDTH) && (y+coord.y >= 0 && y+coord.y < GRID_HEIGHT);
        if(inBounds && grid.at(y+coord.y).at(x+coord.x)) {
            ++count;
        }
    }
    return count;
}
void Game::set(int x, int y, bool active) {
    this->cells.at(y).at(x) = active;
}
bool Game::get(int x, int y) {
    return this->cells.at(y).at(x);
}