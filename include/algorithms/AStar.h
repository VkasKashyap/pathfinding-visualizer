#pragma once
#include "grid/Grid.h"
#include <SFML/Graphics.hpp>
#include <string>

// NEW: Enum to track which math equation we are using
enum class HeuristicType {
    Manhattan,
    Euclidean,
    Chebyshev
};

class AStar {
public:
    static void run(Grid& grid, sf::RenderWindow& window, bool animate = true);
    
    // NEW: UI Helper functions
    static void cycleHeuristic();
    static std::string getCurrentHeuristicName();

private:
    static HeuristicType currentHeuristic;
    // Changed return type to double because Euclidean uses square roots (decimals)
    static double getHeuristic(Node* a, Node* b); 
};