#include "Init.h"
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>
#include <iostream>

EngineInit::EngineInit() {
  if (initSystems() != 0)
    return;
  if (createWindow() != 0)
    return;
  if (createRenderer() != 0)
    return;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
  ImGui_ImplSDLRenderer3_Init(m_renderer);
};

int EngineInit::initSystems() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }
  return 0;
}

int EngineInit::createWindow() {
  m_window = SDL_CreateWindow("Proto-Engine", 800, 600, 0);
  if (!m_window) {
    std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
    return 1;
  }
  return 0;
}

int EngineInit::createRenderer() {
  m_renderer = SDL_CreateRenderer(m_window, nullptr);
  if (!m_renderer) {
    std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
    return 1;
  }
  return 0;
}
