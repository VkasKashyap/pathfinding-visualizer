#include "algorithms/BFS.h"
#include <queue>
#include <thread>
#include <chrono>

void BFS::run(Grid& grid, sf::RenderWindow& window, bool animate) {
    sf::Vector2i startPos = grid.getStartPos();
    sf::Vector2i endPos = grid.getEndPos();

    if (startPos.x == -1 || endPos.x == -1) return;

    Node* startNode = grid.getNode(startPos.x, startPos.y);
    Node* endNode = grid.getNode(endPos.x, endPos.y);

    std::queue<Node*> q;
    q.push(startNode);
    bool found = false;

    while (!q.empty()) {
        if (animate) {
            while (const std::optional<sf::Event> event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) { window.close(); return; }
            }
        }

        Node* current = q.front();
        q.pop();

        if (current == endNode) { found = true; break; }

        std::vector<Node*> neighbors = grid.getNeighbors(current->getGridX(), current->getGridY());
        for (Node* neighbor : neighbors) {
            if (neighbor->getType() == NodeType::Empty || neighbor->getType() == NodeType::End) {
                neighbor->parent = current;
                if (neighbor != endNode) neighbor->setType(NodeType::Visited);
                q.push(neighbor);
            }
        }

        if (animate) {
            window.clear(sf::Color(40, 40, 40));
            grid.draw(window);
            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    if (found) {
        Node* currentPathNode = endNode->parent;
        while (currentPathNode != startNode && currentPathNode != nullptr) {
            currentPathNode->setType(NodeType::Path);
            currentPathNode = currentPathNode->parent;
            
            if (animate) {
                window.clear(sf::Color(40, 40, 40));
                grid.draw(window);
                window.display();
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
    }
}