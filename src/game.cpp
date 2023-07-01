#include "game.h"
#include "constants.h"
#include <array>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <cmath>

Game::Game() {
    this->aliveCells = std::vector<Cell>();
    this->vertices = sf::VertexArray(sf::Quads, 40000);
}
void Game::updateVertexArray() {
    this->vertices.clear();
    this->vertices.resize(40000);
    for(int i = 0; i < this->aliveCells.size(); i++) {
        Cell cell = this->aliveCells.at(i);
        this->vertices[i*4].position = sf::Vector2f((cell.x+xDisplayOffset) * SQUARE_SIZE, (cell.y+yDisplayOffset) * SQUARE_SIZE);
        this->vertices[i*4+1].position = sf::Vector2f((cell.x+xDisplayOffset+1) * SQUARE_SIZE, (cell.y+yDisplayOffset) * SQUARE_SIZE);
        this->vertices[i*4+2].position = sf::Vector2f((cell.x+xDisplayOffset+1) * SQUARE_SIZE, (cell.y+1+yDisplayOffset) * SQUARE_SIZE);
        this->vertices[i*4+3].position = sf::Vector2f((cell.x+xDisplayOffset) * SQUARE_SIZE, (cell.y+1+yDisplayOffset) * SQUARE_SIZE);
        this->vertices[i*4].color = sf::Color::White;
        this->vertices[i*4+1].color = sf::Color::White;
        this->vertices[i*4+2].color = sf::Color::White;
        this->vertices[i*4+3].color = sf::Color::White;
    }
}
void Game::update() {
    /*
        Any live cell with two or three live neighbours survives.
        Any dead cell with three live neighbours becomes a live cell.
        All other live cells die in the next generation. Similarly, all other dead cells stay dead. 
    */
    // loop through live cells, check neighbours in copy
    // if n>1 and n<4, do nothing [yes]
    // if n>3 or n<2, kill cell [yes]
    // check number of neighbours in neighbouring cells, give life to cell if neighbours of neighbour = 3
    std::vector<Cell> aliveCellsCopy;
    std::copy(
        this->aliveCells.begin(),
        this->aliveCells.end(),
        std::back_inserter(aliveCellsCopy)
    );
    auto iter = std::remove_if(this->aliveCells.begin(), this->aliveCells.end(), [this, &aliveCellsCopy](Cell cell) {
        int neighbourCount = this->getAliveNeighbourCount(aliveCellsCopy, cell);
        std::cout << neighbourCount << std::endl;
        return (neighbourCount != 2) && (neighbourCount != 3);
    });
    this->aliveCells.erase(iter, this->aliveCells.end());
    for(Cell cell: aliveCellsCopy) {
        // cells that might have died can still reproduce in the same generation, this is why we loop through aliveCellsCopy instead of this->aliveCells
        std::vector<Cell> neighbours = this->getAllNeighbors(cell);
        for(Cell neighbour: neighbours) {
            int neighbourNeighbourCount = this->getAliveNeighbourCount(aliveCellsCopy, neighbour);
            if(neighbourNeighbourCount == 3 && !this->cellIsAlive(neighbour)) {
                // give life to the neighbouring cell
                this->aliveCells.push_back(neighbour);
            }
        }
    }
}
bool Game::cellIsAlive(Cell cell) {
    return std::find_if(
        this->aliveCells.begin(),
        this->aliveCells.end(),
        [cell](Cell other) {
            return other.x == cell.x && other.y == cell.y;
        }
    ) != this->aliveCells.end();
}
int Game::getAliveNeighbourCount(std::vector<Cell>& cells, Cell cell) {
    std::vector<Cell> neighbourCells;
    return std::count_if(cells.begin(), cells.end(), [cell](Cell other) {
        return !((other.x == cell.x) && (other.y == cell.y)) && (std::abs(other.x-cell.x) < 2) && (std::abs(other.y-cell.y) < 2);
    });
}

std::vector<Cell> Game::getAllNeighbors(Cell cell) {
    // This returns all neighbouring cells, regardless of whether they are alive or not.

    // # # # (-1, 1), (0, 1), (1, 1)
    // #   # (-1, 0), (1, 0)
    // # # # (-1, -1), (0, -1), (1, -1)
    std::vector<Cell> neighbourCells {
        Cell {cell.x - 1, cell.y + 1},
        Cell {cell.x, cell.y + 1},
        Cell {cell.x + 1, cell.y + 1},
        Cell {cell.x - 1, cell.y},
        Cell {cell.x + 1, cell.y},
        Cell {cell.x -1, cell.y - 1},
        Cell {cell.x, cell.y - 1},
        Cell {cell.x + 1, cell.y - 1},
    };
    return neighbourCells;
}
void Game::set(int x, int y, bool active) {
    if(active && !this->get(x, y))
        this->aliveCells.push_back(Cell {x, y});
    else if(!active) {
        auto iter = std::remove_if(
            this->aliveCells.begin(),
            this->aliveCells.end(),
            [x, y](Cell cell) {
                return cell.x == x && cell.y == y;
            }
        );
        this->aliveCells.erase(iter);
    }
}
bool Game::get(int x, int y) {
    return std::find_if(
        this->aliveCells.begin(),
        this->aliveCells.end(),
        [x, y](Cell cell) {
            return cell.x == x && cell.y == y;
        }
    ) != this->aliveCells.end();
}