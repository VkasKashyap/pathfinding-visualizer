#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <string>
#include <fstream>
#include <chrono>
#include <iostream>

#include "grid/Grid.h"
#include "algorithms/BFS.h"
#include "algorithms/DFS.h"
#include "algorithms/AStar.h"
#include "algorithms/MazeGenerator.h"
#include "algorithms/AlgorithmRace.h"

// --- CSV EXPORT FUNCTION ---
void saveBenchmark(const std::string& algorithm, int explored, int pathLength, double timeMs) {
    std::ofstream file("benchmark_results.csv", std::ios::app);
    file.seekp(0, std::ios::end);
    if (file.tellp() == 0) {
        file << "Algorithm,Nodes Explored,Path Length,Time (ms)\n";
    }
    file << algorithm << "," << explored << "," << pathLength << "," << timeMs << "\n";
    file.close();
    std::cout << "Saved " << algorithm << " benchmark to CSV!\n";
}

int main() {
    sf::VideoMode videoMode({800, 600});
    sf::RenderWindow window(videoMode, "Advanced Algorithmic Pathfinding Visualizer");
    window.setFramerateLimit(60);

    Grid grid(800, 600, 20.0f);

    sf::Font font;
    // Load standard Windows font
    font.openFromFile("C:/Windows/Fonts/arial.ttf"); 
    sf::Text uiText(font, "Loading...", 18);
    uiText.setFillColor(sf::Color::White);
    uiText.setPosition({10.0f, 10.0f});

    // Dark semi-transparent background for the UI text
    sf::RectangleShape uiBackground({400.0f, 150.0f}); 
    uiBackground.setFillColor(sf::Color(0, 0, 0, 200)); 
    uiBackground.setPosition({0.0f, 0.0f});

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            
            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::S) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    grid.handleMouseClick(mousePos.x, mousePos.y, NodeType::Start);
                }
                else if (keyPress->code == sf::Keyboard::Key::E) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    grid.handleMouseClick(mousePos.x, mousePos.y, NodeType::End);
                }
                else if (keyPress->code == sf::Keyboard::Key::Num1) {
                    grid.clearPath(); 
                    BFS::run(grid, window);
                }
                else if (keyPress->code == sf::Keyboard::Key::Num2) {
                    grid.clearPath(); 
                    DFS::run(grid, window); 
                }
                else if (keyPress->code == sf::Keyboard::Key::Num3) {
                    grid.clearPath(); 
                    AStar::run(grid, window);
                }
                else if (keyPress->code == sf::Keyboard::Key::M) {
                    MazeGenerator::generateRecursiveBacktracking(grid, window);
                }
                // --- CYCLE A* HEURISTIC ---
                else if (keyPress->code == sf::Keyboard::Key::H) {
                    AStar::cycleHeuristic();
                }
                else if (keyPress->code == sf::Keyboard::Key::V) {
                    AlgorithmRace::run(grid, window);
                }
                // --- RUN BENCHMARK SUITE ---
                else if (keyPress->code == sf::Keyboard::Key::B) {
                    std::cout << "Running Benchmark Suite...\n";
                    
                    grid.clearPath();
                    auto start = std::chrono::high_resolution_clock::now();
                    BFS::run(grid, window, false);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> time = end - start;
                    saveBenchmark("BFS", grid.getVisitedCount(), grid.getPathLength(), time.count());

                    grid.clearPath();
                    start = std::chrono::high_resolution_clock::now();
                    DFS::run(grid, window, false);
                    end = std::chrono::high_resolution_clock::now();
                    time = end - start;
                    saveBenchmark("DFS", grid.getVisitedCount(), grid.getPathLength(), time.count());

                    grid.clearPath();
                    start = std::chrono::high_resolution_clock::now();
                    AStar::run(grid, window, false);
                    end = std::chrono::high_resolution_clock::now();
                    time = end - start;
                    // I also updated this to log exactly WHICH heuristic A* used in the CSV!
                    saveBenchmark("A* (" + AStar::getCurrentHeuristicName() + ")", grid.getVisitedCount(), grid.getPathLength(), time.count());
                    
                    // Re-run A* visibly so the user isn't looking at a blank screen
                    grid.clearPath();
                    AStar::run(grid, window, true);
                }
                else if (keyPress->code == sf::Keyboard::Key::Space || keyPress->code == sf::Keyboard::Key::C) {
                    grid.clearPath();
                }
                else if (keyPress->code == sf::Keyboard::Key::R) {
                    grid.clearBoard();
                }
            }
        }

        // --- MOUSE CONTROLS ---
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            grid.handleMouseClick(mousePos.x, mousePos.y, NodeType::Obstacle);
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            grid.handleMouseClick(mousePos.x, mousePos.y, NodeType::Empty);
        }

        // --- REAL-TIME UI UPDATES ---
        std::string info = "Controls: [S] Start | [E] End | [L/R Click] Walls\n";
        info += "Run: [1] BFS | [2] DFS | [3] A*\n";
        info += "Tools: [M] Maze | [B] Benchmark | [V] Versus Race!\n";
        info += "A* Heuristic [H]: " + AStar::getCurrentHeuristicName() + "\n";
        info += "Nodes Explored: " + std::to_string(grid.getVisitedCount()) + "\n";
        info += "Path Length: " + std::to_string(grid.getPathLength());
        uiText.setString(info);

        // --- RENDER CYCLE ---
        window.clear(sf::Color(40, 40, 40));
        
        grid.draw(window);
        window.draw(uiBackground);
        window.draw(uiText);
        
        window.display();
    }

    return 0;
}