#pragma once
#include "grid/Grid.h"
#include <SFML/Graphics.hpp>

class BFS {
public:
    static void run(Grid& grid, sf::RenderWindow& window, bool animate = true);
};