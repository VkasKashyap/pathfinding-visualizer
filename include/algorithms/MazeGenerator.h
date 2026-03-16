#pragma once
#include "grid/Grid.h"
#include <SFML/Graphics.hpp>

class MazeGenerator {
public:
    static void generateRecursiveBacktracking(Grid& grid, sf::RenderWindow& window);
};