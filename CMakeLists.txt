cmake_minimum_required(VERSION 3.10)
project(PathfindingVisualizer VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the NEW SFML 3 graphics package
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# Tell the compiler where to look for header files
include_directories(include)

add_executable(PathfindingVisualizer 
    src/main.cpp 
    src/grid/Node.cpp
    src/grid/Grid.cpp
    src/algorithms/BFS.cpp
    src/algorithms/DFS.cpp
    src/algorithms/AStar.cpp
    src/algorithms/MazeGenerator.cpp
    src/algorithms/AlgorithmRace.cpp
)

# Link the SFML 3 libraries to our program
target_link_libraries(PathfindingVisualizer SFML::Graphics SFML::Window SFML::System)


# ... existing target_link_libraries(PathfindingVisualizer ...) ...

# --- NEW: GOOGLE TEST SETUP ---
# 1. Enable testing in this CMake project
enable_testing()

# 2. Find the Google Test framework we installed via MSYS2
find_package(GTest REQUIRED)

# 3. Create a brand new executable specifically for running tests
add_executable(RunTests 
    tests/node_tests.cpp 
    src/grid/Node.cpp
)

# 4. Link GTest and SFML to our test runner
target_link_libraries(RunTests 
    GTest::gtest 
    GTest::gtest_main 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# 5. Register the test with CMake
add_test(NAME NodeUnitTests COMMAND RunTests)