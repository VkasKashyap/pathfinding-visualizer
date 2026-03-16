#include "algorithms/AlgorithmRace.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <thread>
#include <chrono>

void AlgorithmRace::run(const Grid& initialGrid, sf::RenderWindow& window) {
    // 1. Create two independent copies of the user's maze
    Grid gridBFS = initialGrid;
    Grid gridAStar = initialGrid;
    gridBFS.clearPath();
    gridAStar.clearPath();

    // 2. Setup BFS State
    std::queue<Node*> qBFS;
    Node* startBFS = gridBFS.getNode(gridBFS.getStartPos().x, gridBFS.getStartPos().y);
    Node* endBFS = gridBFS.getNode(gridBFS.getEndPos().x, gridBFS.getEndPos().y);
    if (startBFS) qBFS.push(startBFS);
    bool bfsFound = false;
    Node* bfsPathNode = nullptr;

    // 3. Setup A* State
    std::unordered_map<Node*, double> gScore;
    std::unordered_map<Node*, double> fScore;
    Node* startAStar = gridAStar.getNode(gridAStar.getStartPos().x, gridAStar.getStartPos().y);
    Node* endAStar = gridAStar.getNode(gridAStar.getEndPos().x, gridAStar.getEndPos().y);
    
    auto getHeuristic = [](Node* a, Node* b) {
        return std::abs(a->getGridX() - b->getGridX()) + std::abs(a->getGridY() - b->getGridY());
    };

    if (startAStar) {
        gScore[startAStar] = 0;
        fScore[startAStar] = getHeuristic(startAStar, endAStar);
    }
    auto cmp = [&fScore](Node* left, Node* right) { return fScore[left] > fScore[right]; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> qAStar(cmp);
    std::unordered_map<Node*, bool> inOpenSet;

    if (startAStar) {
        qAStar.push(startAStar);
        inOpenSet[startAStar] = true;
    }
    bool astarFound = false;
    Node* astarPathNode = nullptr;

    // 4. Setup SFML Split-Screen Viewports
    sf::View viewLeft(sf::FloatRect({0, 0}, {800, 600}));
    viewLeft.setViewport(sf::FloatRect({0.f, 0.f}, {0.5f, 1.f})); // Left 50%
    
    sf::View viewRight(sf::FloatRect({0, 0}, {800, 600}));
    viewRight.setViewport(sf::FloatRect({0.5f, 0.f}, {0.5f, 1.f})); // Right 50%

    // Text labels
    sf::Font font;
    font.openFromFile("C:/Windows/Fonts/arial.ttf");
    sf::Text textLeft(font, "BFS Search", 24); textLeft.setPosition({10, 10});
    sf::Text textRight(font, "A* Search", 24); textRight.setPosition({410, 10});

    // 5. THE DUAL LOOP
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) { window.close(); return; }
        }

        bool activity = false;

        // Step BFS forward by 1
        if (!bfsFound && !qBFS.empty()) {
            activity = true;
            Node* current = qBFS.front();
            qBFS.pop();

            if (current == endBFS) { bfsFound = true; bfsPathNode = endBFS->parent; }
            else {
                for (Node* neighbor : gridBFS.getNeighbors(current->getGridX(), current->getGridY())) {
                    if (neighbor->getType() == NodeType::Empty || neighbor->getType() == NodeType::End) {
                        neighbor->parent = current;
                        if (neighbor != endBFS) neighbor->setType(NodeType::Visited);
                        qBFS.push(neighbor);
                    }
                }
            }
        } else if (bfsFound && bfsPathNode && bfsPathNode != startBFS) {
            activity = true;
            bfsPathNode->setType(NodeType::Path);
            bfsPathNode = bfsPathNode->parent;
        }

        // Step A* forward by 1
        if (!astarFound && !qAStar.empty()) {
            activity = true;
            Node* current = qAStar.top();
            qAStar.pop();
            inOpenSet[current] = false;

            if (current == endAStar) { astarFound = true; astarPathNode = endAStar->parent; }
            else {
                if (current != startAStar) current->setType(NodeType::Visited);
                for (Node* neighbor : gridAStar.getNeighbors(current->getGridX(), current->getGridY())) {
                    if (neighbor->getType() == NodeType::Obstacle) continue;
                    double tenG = gScore[current] + 1.0;
                    if (gScore.find(neighbor) == gScore.end() || tenG < gScore[neighbor]) {
                        neighbor->parent = current;
                        gScore[neighbor] = tenG;
                        fScore[neighbor] = tenG + getHeuristic(neighbor, endAStar);
                        if (!inOpenSet[neighbor]) {
                            qAStar.push(neighbor);
                            inOpenSet[neighbor] = true;
                            if (neighbor != endAStar) neighbor->setType(NodeType::Open);
                        }
                    }
                }
            }
        } else if (astarFound && astarPathNode && astarPathNode != startAStar) {
            activity = true;
            astarPathNode->setType(NodeType::Path);
            astarPathNode = astarPathNode->parent;
        }

        // --- DRAWING ---
        window.clear(sf::Color(20, 20, 20));

        // Draw Left (BFS)
        window.setView(viewLeft);
        gridBFS.draw(window);

        // Draw Right (AStar)
        window.setView(viewRight);
        gridAStar.draw(window);

        // Draw UI overlay
        window.setView(window.getDefaultView());
        sf::RectangleShape divider({4.0f, 600.0f});
        divider.setPosition({398.0f, 0.0f});
        window.draw(divider);
        window.draw(textLeft);
        window.draw(textRight);

        window.display();

        if (!activity) {
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause on finish
            break; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}