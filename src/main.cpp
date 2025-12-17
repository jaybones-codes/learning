#include "Input.h"
#include "TimeManager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
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
  float rectW = 50;
  float rectH = 50;
  float rectX = 100.0f;
  float rectY = 100.0f;
  float speed = 200.0f; // pixels per second
  while (isRunning) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      input.processEvent(event);
      if (event.type == SDL_EVENT_QUIT) {
        isRunning = false;
      }
    }
    time.updateDeltaTime(); // Actual Loop
    float dt = time.getDeltaTime();
    if (input.isKeyDown(SDL_SCANCODE_S)) {

      rectY += speed * dt;
    }
    if (input.isKeyDown(SDL_SCANCODE_W)) {

      rectY -= speed * dt;
    }
    if (input.isKeyDown(SDL_SCANCODE_A)) {

      rectX -= speed * dt;
    }
    if (input.isKeyDown(SDL_SCANCODE_D)) {

      rectX += speed * dt;
    }
    if (input.isMouseButtonPressed(SDL_BUTTON_LEFT)) {
      std::cout << "Mouse clicked!" << std::endl;
      rectX = input.getMouseX();
      rectY = input.getMouseY();
      std::cout << "Target: " << rectX << ", " << rectY << std::endl;
    } // Clear screen and present
    input.update();

    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);
    SDL_FRect rect = {rectX, rectY, rectW, rectH};
    SDL_SetRenderDrawColor(renderer, 55, 66, 11, 255);

    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);

  } // Clean Up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  std::cout << "Engine shutdown";
  return 0;
}
