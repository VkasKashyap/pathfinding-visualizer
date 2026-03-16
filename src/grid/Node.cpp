#include "grid/Node.h"

// Constructor implementation
Node::Node(int gridX, int gridY, float size)
    : m_gridX(gridX), m_gridY(gridY), m_size(size), m_type(NodeType::Empty) {
    
    // Set up the visual square
    // We subtract 1.0f from the size to leave a 1-pixel gap for grid lines!
    m_shape.setSize({size - 1.0f, size - 1.0f}); 
    m_shape.setPosition({gridX * size, gridY * size});
    updateColor();
}

// Getters
int Node::getGridX() const { return m_gridX; }
int Node::getGridY() const { return m_gridY; }
NodeType Node::getType() const { return m_type; }

// Setters
void Node::setType(NodeType type) {
    m_type = type;
    updateColor(); // Automatically change color when the type changes
}

// Drawing function
void Node::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}

// Color logic based on the node's state
void Node::updateColor() {
    switch (m_type) {
        case NodeType::Empty:    m_shape.setFillColor(sf::Color::White); break;
        case NodeType::Obstacle: m_shape.setFillColor(sf::Color(100, 100, 100)); break;
        case NodeType::Start:    m_shape.setFillColor(sf::Color::Green); break;
        case NodeType::End:      m_shape.setFillColor(sf::Color::Red); break;
        case NodeType::Visited:  m_shape.setFillColor(sf::Color::Cyan); break;
        case NodeType::Path:     m_shape.setFillColor(sf::Color::Yellow); break;
        case NodeType::Open:     m_shape.setFillColor(sf::Color::Blue); break; // NEW!
    }
}