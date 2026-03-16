#pragma once
#include "grid/Grid.h"
#include <SFML/Graphics.hpp>

class AlgorithmRace {
public:
    // Notice we pass the Grid by reference (const Grid&) to safely copy it inside
    static void run(const Grid& initialGrid, sf::RenderWindow& window);
};