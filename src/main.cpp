#include "Camera.h"
#include "ECS.h"
#include "Input.h"
#include "TileGrid.h"
#include "TimeManager.h"
#include <SDL3/SDL.h>
#include <iostream>

int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Proto-Engine", 800, 600, 0);
  if (!window) {
    std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
    return 1;
  }
  EntityManager em;
  std::cout << em.createEntity() << " created Entity" << std::endl;
  std::cout << em.createEntity() << " created Entity" << std::endl;
  std::cout << em.createEntity() << " created Entity" << std::endl;
  em.destroyEntity(2);

  std::cout << em.createEntity() << " created Entity" << std::endl;
  std::cout << em.createEntity() << " created Entity" << std::endl;
  bool isRunning = true;
  TimeManager time;
  Input input;
  TileGrid tg;
  Camera camera(800, 600, tg.GRID_WIDTH * tg.TILE_SIZE,
                tg.GRID_HEIGHT * tg.TILE_SIZE);

  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      input.processEvent(event);
      if (event.type == SDL_EVENT_QUIT) {
        isRunning = false;
      }
    }

    time.updateDeltaTime();
    float dt = time.getDeltaTime();

    camera.update(dt, input);

    // Handle brush switching
    if (input.isKeyPressed(SDL_SCANCODE_1))
      tg.setCurrentBrush(TileType::Floor);
    if (input.isKeyPressed(SDL_SCANCODE_2))
      tg.setCurrentBrush(TileType::Wall);
    if (input.isKeyPressed(SDL_SCANCODE_3))
      tg.setCurrentBrush(TileType::Water);
    if (input.isKeyPressed(SDL_SCANCODE_4))
      tg.setCurrentBrush(TileType::Grass);

    // Handle painting with camera offset
    // In main.cpp, in the painting section:
    if (input.isMouseButtonDown(SDL_BUTTON_LEFT)) {
      int worldX = input.getMouseX() + (int)camera.getX();
      int worldY = input.getMouseY() + (int)camera.getY();
      int gridX = worldX / tg.TILE_SIZE;
      int gridY = worldY / tg.TILE_SIZE;

      std::cout << "Mouse: " << input.getMouseX() << ", " << input.getMouseY()
                << std::endl;
      std::cout << "World: " << worldX << ", " << worldY << std::endl;
      std::cout << "Grid: " << gridX << ", " << gridY << std::endl;
      std::cout << "Brush: " << (int)tg.getCurrentBrush() << std::endl;

      tg.setTileType(gridX, gridY, tg.getCurrentBrush());
    }

    input.update();

    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    tg.renderTileGrid(renderer, camera.getX(), camera.getY());
    tg.renderLineGrid(renderer, camera.getX(), camera.getY());

    int worldMouseX = input.getMouseX() + (int)camera.getX();
    int worldMouseY = input.getMouseY() + (int)camera.getY();
    tg.tileHighlight(renderer, worldMouseX, worldMouseY, camera.getX(),
                     camera.getY());

    tg.renderBrushIndicator(renderer);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
