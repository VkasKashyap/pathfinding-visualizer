#include "algorithms/DFS.h"
#include <stack>
#include <thread>
#include <chrono>

void DFS::run(Grid& grid, sf::RenderWindow& window, bool animate) {
    sf::Vector2i startPos = grid.getStartPos();
    sf::Vector2i endPos = grid.getEndPos();

    if (startPos.x == -1 || endPos.x == -1) return;

    Node* startNode = grid.getNode(startPos.x, startPos.y);
    Node* endNode = grid.getNode(endPos.x, endPos.y);

    std::stack<Node*> s;
    s.push(startNode);
    bool found = false;

    while (!s.empty()) {
        if (animate) {
            while (const std::optional<sf::Event> event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) { window.close(); return; }
            }
        }

        Node* current = s.top();
        s.pop();

        if (current == endNode) { found = true; break; }

        if (current->getType() != NodeType::Visited && current != startNode) {
            current->setType(NodeType::Visited);
            if (animate) {
                window.clear(sf::Color(40, 40, 40));
                grid.draw(window);
                window.display();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        std::vector<Node*> neighbors = grid.getNeighbors(current->getGridX(), current->getGridY());
        for (Node* neighbor : neighbors) {
            if (neighbor->getType() == NodeType::Empty || neighbor->getType() == NodeType::End) {
                if (neighbor->parent == nullptr) {
                    neighbor->parent = current;
                }
                s.push(neighbor);
            }
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