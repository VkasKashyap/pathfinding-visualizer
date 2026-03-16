#include "algorithms/MazeGenerator.h"
#include <stack>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

void MazeGenerator::generateRecursiveBacktracking(Grid& grid, sf::RenderWindow& window) {
    grid.clearBoard();
    int cols = grid.getCols();
    int rows = grid.getRows();

    // 1. Fill the entire board with solid walls
    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            grid.getNode(x, y)->setType(NodeType::Obstacle);
        }
    }

    // 2. Setup the DFS Stack and Random Number Generator
    std::stack<Node*> s;
    Node* startNode = grid.getNode(1, 1);
    startNode->setType(NodeType::Empty);
    s.push(startNode);

    std::random_device rd;
    std::mt19937 gen(rd());

    // 3. Carve the maze
    while (!s.empty()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) { window.close(); return; }
        }

        Node* current = s.top();
        s.pop();

        int cx = current->getGridX();
        int cy = current->getGridY();

        // Check neighbors that are TWO steps away (Up, Down, Left, Right)
        std::vector<Node*> neighbors;
        std::vector<std::pair<int, int>> dirs = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};
        
        for (auto dir : dirs) {
            int nx = cx + dir.first;
            int ny = cy + dir.second;
            
            // Keep a 1-block border around the screen
            if (nx > 0 && nx < cols - 1 && ny > 0 && ny < rows - 1) {
                Node* neighbor = grid.getNode(nx, ny);
                // If the neighbor is still a solid wall, it's a valid place to carve!
                if (neighbor->getType() == NodeType::Obstacle) {
                    neighbors.push_back(neighbor);
                }
            }
        }

        if (!neighbors.empty()) {
            // We found valid neighbors, so push current back onto the stack to remember it
            s.push(current);

            // Pick a completely random neighbor to carve toward
            std::uniform_int_distribution<> dist(0, neighbors.size() - 1);
            Node* next = neighbors[dist(gen)];

            // Turn both the destination node AND the node between them into Empty space
            int nx = next->getGridX();
            int ny = next->getGridY();
            grid.getNode(nx, ny)->setType(NodeType::Empty);
            grid.getNode(cx + (nx - cx) / 2, cy + (ny - cy) / 2)->setType(NodeType::Empty);

            s.push(next);

            // Animate the carving process
            window.clear(sf::Color(40, 40, 40));
            grid.draw(window);
            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}