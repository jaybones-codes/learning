
cmake_minimum_required(VERSION 3.15)

project(proto-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- SDL3 ----
find_package(SDL3 REQUIRED)

# ---- ImGui ----
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

# ---- Your sources (ALL in src/) ----
file(GLOB_RECURSE ENGINE_SOURCES
    src/*.cpp
)

add_executable(proto-engine
    ${ENGINE_SOURCES}
    ${IMGUI_SOURCES}
)

# ---- Include paths ----
target_include_directories(proto-engine PRIVATE
    src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# ---- Link ----
target_link_libraries(proto-engine PRIVATE
    SDL3::SDL3
)

