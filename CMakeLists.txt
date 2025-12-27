cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
project(proto-engine LANGUAGES CXX)

# Collect source files
set(SOURCES
    src/main.cpp
    src/TimeManager.cpp
    src/Input.cpp
    src/TileGrid.cpp
    src/Camera.cpp
    src/Entity.cpp
    src/ECS.cpp
    src/HerdSystem.cpp

)


# Create executable
add_executable(proto-engine ${SOURCES})

# Add include directory for headers
target_include_directories(proto-engine
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
find_package(SDL3 REQUIRED)
target_link_libraries(proto-engine SDL3::SDL3)
