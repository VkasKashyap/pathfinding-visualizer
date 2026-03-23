# Advanced Algorithmic Pathfinding Visualizer

<div align="center">
  <video src="assets/demo.mp4" width="800" controls autoplay loop muted></video>
</div>

## 📌 Overview
An interactive, high-performance pathfinding and graph traversal visualizer built entirely in modern C++ (C++17) using the SFML graphics library. 

This project demonstrates advanced Data Structures & Algorithms (DSA), real-time rendering, memory management, and procedural generation. It was built to strictly analyze and compare the efficiency of various search algorithms in a highly interactive grid environment.

## 🚀 Key Features
* **Algorithm Race Mode (Split-Screen):** Run BFS and A* simultaneously on the exact same maze in a split-screen viewport to visually prove algorithm efficiency.
* **Algorithm Benchmarking Engine:** Run a headless benchmark suite that tests algorithms instantly, logging "Nodes Explored", "Path Length", and exact "Execution Time (ms)" to a `.csv` file.
* **Dynamic A* Heuristics:** Toggle between Manhattan, Euclidean, and Chebyshev mathematical heuristics in real-time to see how they affect A*'s decision-making.
* **Breadth-First Search (BFS):** Guarantees the shortest path by exploring equally in all directions using a `std::queue`.
* **Depth-First Search (DFS):** Explores deeply into a graph before backtracking using a `std::stack`.
* **Procedural Maze Generation:** Uses Recursive Backtracking to carve complex, random mazes out of a solid block of walls.
* **Real-Time UI Dashboard:** Tracks the current heuristic and performance metrics live on-screen.

## 🧠 Algorithm Complexity

| Algorithm | Time Complexity | Space Complexity | Shortest Path? | Data Structure |
| :--- | :--- | :--- | :--- | :--- |
| **BFS** | O(V + E) | O(V) | Yes | `std::queue` |
| **DFS** | O(V + E) | O(V) | No | `std::stack` |
| **A\*** | O(E log V) | O(V) | Yes | `std::priority_queue` |

*(Where V = Vertices/Nodes, and E = Edges)*

## 📊 Example Benchmark Output

Running the benchmark suite (`B`) generates empirical data comparing the memory footprint and execution speed of the algorithms on the current maze:

```csv
Algorithm,Nodes Explored,Path Length,Time (ms)
BFS,1824,34,18.45
DFS,720,67,12.11
A* (Manhattan),312,34,5.02
A* (Euclidean),330,34,5.81

### Part 3: Controls and Architecture
This block explains how to use the app and shows off your clean directory structure.

```markdown
## 🎮 Controls

| Key / Input | Action |
| :--- | :--- |
| **Left Click & Drag** | Draw Obstacle Walls (Grey) |
| **Right Click & Drag** | Erase Walls (White) |
| **Hover + `S`** | Place the Start Node (Green) |
| **Hover + `E`** | Place the End Node (Red) |
| **`1` / `2` / `3`** | Run BFS / Run DFS / Run A* |
| **`H`** | Cycle A* Heuristic (Manhattan, Euclidean, Chebyshev) |
| **`V`** | **Versus Mode!** Split-screen BFS vs A* Race |
| **`B`** | **Benchmark Mode!** Run headless tests & export to CSV |
| **`M`** | Generate a Random Maze |
| **`C` / `Space`** | Clear the current path (Keeps walls/nodes) |
| **`R`** | Hard Reset (Wipes the entire board) |

## 🏗️ Project Architecture

```text
pathfinding-visualizer/
├── assets/                 # Contains demo.mp4 and other media
├── include/                # Header files (.h)
│   ├── algorithms/
│   └── grid/
├── src/                    # Implementation files (.cpp)
│   ├── algorithms/
│   ├── grid/
│   └── main.cpp            # Application entry point and game loop
├── tests/                  # GTest automated testing
├── CMakeLists.txt          # Build configuration
└── README.md

### Part 4: Tech Stack, Build Steps, and Future Plans
This final block proves you know how to configure and build C++ environments.

```markdown
## 🧰 Technologies Used
* **C++17:** Core logic and memory management
* **SFML 3.0+:** Hardware-accelerated 2D graphics and window management
* **CMake:** Build system generation
* **Google Test (GTest):** Automated unit testing framework
* **Standard Template Library (STL):** Queues, stacks, vectors, maps, and chrono timing
* **MinGW-w64 / GCC:** Compiler toolchain

## 🛠️ Build Instructions

### Prerequisites
* **CMake** (Version 3.10 or higher)
* **C++17 Compiler** (e.g., GCC via MSYS2 / MinGW-w64 on Windows)
* **SFML 3.0+**

### Compilation (Windows / MSYS2)
Open your terminal in the root directory of this project and run the following commands:

```bash
# 1. Configure the build system
cmake -B build -G "MinGW Makefiles"

# 2. Compile the executable
cmake --build build

# 3. Run the application
./build/PathfindingVisualizer.exe

## 🧪 Unit Testing

This project utilizes **Google Test** to ensure the reliability of the core grid architecture and state changes. To run the automated test suite:
./build/RunTests.exe



## 🔮 Future Improvements



Planned upgrades include:

* Dijkstra's Algorithm

* Jump Point Search

* Bidirectional A*

* Weighted terrain system (e.g., Mud/Water with higher traversal costs)

* WebAssembly browser version

