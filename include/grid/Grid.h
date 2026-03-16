#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "grid/Node.h"

class Grid {
public:
    Grid(int windowWidth, int windowHeight, float nodeSize);
    void draw(sf::RenderWindow& window) const;
    void handleMouseClick(int mouseX, int mouseY, NodeType type);

    sf::Vector2i getStartPos() const;
    sf::Vector2i getEndPos() const;
    Node* getNode(int x, int y);
    std::vector<Node*> getNeighbors(int x, int y);

    int getCols() const;
    int getRows() const;

    int getVisitedCount() const;
    int getPathLength() const;

    // --- NEW: RESET FUNCTIONS ---
    void clearPath();
    void clearBoard();
    // ----------------------------
    

private:
    int m_width;
    int m_height;
    float m_nodeSize;
    std::vector<std::vector<Node>> m_nodes;

    sf::Vector2i m_startPos{-1, -1}; 
    sf::Vector2i m_endPos{-1, -1};
};