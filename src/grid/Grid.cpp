#include "grid/Grid.h"

Grid::Grid(int windowWidth, int windowHeight, float nodeSize)
    : m_width(windowWidth), m_height(windowHeight), m_nodeSize(nodeSize) {
    
    // Calculate how many columns and rows can fit on the screen
    int cols = windowWidth / nodeSize;
    int rows = windowHeight / nodeSize;

    // Set up the 2D vector
    m_nodes.resize(cols); // Make enough columns
    for (int x = 0; x < cols; ++x) {
        for (int y = 0; y < rows; ++y) {
            // Create a new Node at (x, y) and add it to the column
            m_nodes[x].emplace_back(x, y, nodeSize);
        }
    }
}

void Grid::draw(sf::RenderWindow& window) const {
    // Loop through every column, and every node in that column, and draw it
    for (const auto& column : m_nodes) {
        for (const auto& node : column) {
            node.draw(window);
        }
    }
}

void Grid::handleMouseClick(int mouseX, int mouseY, NodeType type) {
    int gridX = mouseX / m_nodeSize;
    int gridY = mouseY / m_nodeSize;
    int maxCols = m_width / m_nodeSize;
    int maxRows = m_height / m_nodeSize;

    // Safety Check: Are we inside the window?
    if (gridX >= 0 && gridX < maxCols && gridY >= 0 && gridY < maxRows) {
        
        // If placing a Start node, find the old one and reset it to Empty
        if (type == NodeType::Start) {
            if (m_startPos.x != -1) {
                m_nodes[m_startPos.x][m_startPos.y].setType(NodeType::Empty);
            }
            m_startPos = {gridX, gridY}; // Save the new location
        }
        // If placing an End node, find the old one and reset it to Empty
        else if (type == NodeType::End) {
            if (m_endPos.x != -1) {
                m_nodes[m_endPos.x][m_endPos.y].setType(NodeType::Empty);
            }
            m_endPos = {gridX, gridY}; // Save the new location
        }
        // If drawing a Wall or Erasing, check if we accidentally overwrote the Start or End!
        else if (type == NodeType::Obstacle || type == NodeType::Empty) {
            if (m_startPos.x == gridX && m_startPos.y == gridY) m_startPos = {-1, -1};
            if (m_endPos.x == gridX && m_endPos.y == gridY) m_endPos = {-1, -1};
        }

        // Actually change the color of the node we clicked
        m_nodes[gridX][gridY].setType(type);
    }
}


// --- NEW: PATHFINDING HELPERS IMPLEMENTATION ---

sf::Vector2i Grid::getStartPos() const { return m_startPos; }
sf::Vector2i Grid::getEndPos() const { return m_endPos; }

// Safely gets a pointer to a Node, or returns nullptr if out of bounds
Node* Grid::getNode(int x, int y) {
    int maxCols = m_width / m_nodeSize;
    int maxRows = m_height / m_nodeSize;
    
    if (x >= 0 && x < maxCols && y >= 0 && y < maxRows) {
        return &m_nodes[x][y];
    }
    return nullptr;
}

// Finds all valid, non-wall neighbors (Right, Left, Down, Up)
std::vector<Node*> Grid::getNeighbors(int x, int y) {
    std::vector<Node*> neighbors;
    
    // The 4 directions: {X offset, Y offset}
    std::vector<std::pair<int, int>> directions = {
        {1, 0},  // Right
        {-1, 0}, // Left
        {0, 1},  // Down
        {0, -1}  // Up
    };
    
    for (const auto& dir : directions) {
        Node* neighbor = getNode(x + dir.first, y + dir.second);
        
        // If the neighbor exists on the grid, AND it is not a wall
        if (neighbor != nullptr && neighbor->getType() != NodeType::Obstacle) {
            neighbors.push_back(neighbor);
        }
    }
    
    return neighbors;
}


// --- NEW: RESET FUNCTION IMPLEMENTATION ---

void Grid::clearPath() {
    for (auto& column : m_nodes) {
        for (auto& node : column) {
            node.parent = nullptr; 
            // Added NodeType::Open to the cleanup list!
            if (node.getType() == NodeType::Visited || node.getType() == NodeType::Path || node.getType() == NodeType::Open) {
                node.setType(NodeType::Empty);
            }
        }
    }
}

void Grid::clearBoard() {
    // Loop through and wipe absolutely everything
    for (auto& column : m_nodes) {
        for (auto& node : column) {
            node.parent = nullptr;
            node.setType(NodeType::Empty);
        }
    }
    // Forget where the Start and End nodes were
    m_startPos = {-1, -1};
    m_endPos = {-1, -1};
}

int Grid::getCols() const { return m_width / m_nodeSize; }
int Grid::getRows() const { return m_height / m_nodeSize; }


// --- NEW: BENCHMARK IMPLEMENTATIONS ---

int Grid::getVisitedCount() const {
    int count = 0;
    for (const auto& column : m_nodes) {
        for (const auto& node : column) {
            // Count nodes that were checked by the algorithm (Cyan, Blue, or Yellow)
            if (node.getType() == NodeType::Visited || 
                node.getType() == NodeType::Open || 
                node.getType() == NodeType::Path) {
                count++;
            }
        }
    }
    return count;
}

int Grid::getPathLength() const {
    int count = 0;
    for (const auto& column : m_nodes) {
        for (const auto& node : column) {
            // Only count the final Yellow path
            if (node.getType() == NodeType::Path) {
                count++;
            }
        }
    }
    return count;
}