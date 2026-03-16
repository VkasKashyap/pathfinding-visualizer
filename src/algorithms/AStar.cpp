#include "algorithms/AStar.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <thread>
#include <chrono>

// Initialize the static variable to Manhattan by default
HeuristicType AStar::currentHeuristic = HeuristicType::Manhattan;

void AStar::cycleHeuristic() {
    if (currentHeuristic == HeuristicType::Manhattan) currentHeuristic = HeuristicType::Euclidean;
    else if (currentHeuristic == HeuristicType::Euclidean) currentHeuristic = HeuristicType::Chebyshev;
    else currentHeuristic = HeuristicType::Manhattan;
}

std::string AStar::getCurrentHeuristicName() {
    switch (currentHeuristic) {
        case HeuristicType::Manhattan: return "Manhattan";
        case HeuristicType::Euclidean: return "Euclidean";
        case HeuristicType::Chebyshev: return "Chebyshev";
        default: return "Unknown";
    }
}

// NEW: The Heuristic Engine Math
double AStar::getHeuristic(Node* a, Node* b) {
    double dx = std::abs(a->getGridX() - b->getGridX());
    double dy = std::abs(a->getGridY() - b->getGridY());

    switch (currentHeuristic) {
        case HeuristicType::Manhattan:
            return dx + dy;
        case HeuristicType::Euclidean:
            return std::sqrt(dx * dx + dy * dy);
        case HeuristicType::Chebyshev:
            return std::max(dx, dy);
        default:
            return dx + dy;
    }
}

void AStar::run(Grid& grid, sf::RenderWindow& window, bool animate) {
    sf::Vector2i startPos = grid.getStartPos();
    sf::Vector2i endPos = grid.getEndPos();
    if (startPos.x == -1 || endPos.x == -1) return;

    Node* startNode = grid.getNode(startPos.x, startPos.y);
    Node* endNode = grid.getNode(endPos.x, endPos.y);

    // Changed int to double to handle decimal math from Euclidean
    std::unordered_map<Node*, double> gScore;
    std::unordered_map<Node*, double> fScore;

    gScore[startNode] = 0.0;
    fScore[startNode] = getHeuristic(startNode, endNode);

    auto cmp = [&fScore](Node* left, Node* right) { return fScore[left] > fScore[right]; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openSet(cmp);
    std::unordered_map<Node*, bool> inOpenSet;

    openSet.push(startNode);
    inOpenSet[startNode] = true;

    bool found = false;

    while (!openSet.empty()) {
        if (animate) {
            while (const std::optional<sf::Event> event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) { window.close(); return; }
            }
        }

        Node* current = openSet.top();
        openSet.pop();
        inOpenSet[current] = false;

        if (current == endNode) { found = true; break; }

        if (current != startNode) {
            current->setType(NodeType::Visited);
        }

        std::vector<Node*> neighbors = grid.getNeighbors(current->getGridX(), current->getGridY());
        for (Node* neighbor : neighbors) {
            if (neighbor->getType() == NodeType::Obstacle) continue;

            double tentative_gScore = gScore[current] + 1.0;

            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                neighbor->parent = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = tentative_gScore + getHeuristic(neighbor, endNode);

                if (!inOpenSet[neighbor]) {
                    openSet.push(neighbor);
                    inOpenSet[neighbor] = true;
                    if (neighbor != endNode) {
                        neighbor->setType(NodeType::Open);
                    }
                }
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