#include "Input.h"
#include "TimeManager.h"
#include <SDL3/SDL.h>
#include <iostream>

int main() {
  bool isRunning = true;

  if (!SDL_Init(SDL_INIT_VIDEO)) {

    std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  std::cout << "SDL initialized successfully" << std::endl;

  SDL_Window *window = SDL_CreateWindow("Proto-Engine", 800, 600, 0);
  if (window == nullptr) {
    std::cerr << "Error: Failed to create Window: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
  if (renderer == nullptr) {
    std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
  }
  std::cout << "Window created successfully" << std::endl;
  std::cout << "Entering game loop..." << std::endl;

  TimeManager time;
  Input input;
  // Game Loop

  while (isRunning) {
    input.update();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      input.processEvent(event);
      if (event.type == SDL_EVENT_QUIT) {
        isRunning = false;
      }
    }

    time.updateDeltaTime(); // Actual Loop
    // std::cout << "Delta Time " << time.getDeltaTime() << std::endl;//debug
    // shit

    // Clear screen and present
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

  } // Clean Up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  std::cout << "Engine shutdown";
  return 0;
}
