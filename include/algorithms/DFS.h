#pragma once
#include "grid/Grid.h"
#include <SFML/Graphics.hpp>

class DFS {
public:
    static void run(Grid& grid, sf::RenderWindow& window, bool animate = true);
};