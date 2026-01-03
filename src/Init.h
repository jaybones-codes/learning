#pragma once
#include <SDL3/SDL.h>

class EngineInit {
public:
  EngineInit();

private:
  SDL_Window *m_window = nullptr;
  SDL_Renderer *m_renderer = nullptr;

public:
  SDL_Window *getWindow() { return m_window; };
  SDL_Renderer *getRenderer() { return m_renderer; };

  int initSystems();
  int createWindow();
  int createRenderer();
};
