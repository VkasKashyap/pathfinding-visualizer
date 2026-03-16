#pragma once
#include <SFML/Graphics.hpp>

enum class NodeType {
    Empty, Obstacle, Start, End, Visited, Path, Open // Added 'Open' here!
};

class Node {
public:
    Node(int gridX, int gridY, float size);

    int getGridX() const;
    int getGridY() const;
    NodeType getType() const;
    void setType(NodeType type);
    void draw(sf::RenderWindow& window) const;

    // --- NEW: PATHFINDING DATA ---
    // A pointer to the node that discovered this one
    Node* parent = nullptr; 
    // -----------------------------

private:
    int m_gridX;
    int m_gridY;
    float m_size;
    NodeType m_type;
    sf::RectangleShape m_shape;
    
    void updateColor();
};