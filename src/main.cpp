#include "Camera.h"
#include "ECS.h"
#include "HerdSystem.h"
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
  ComponentManager cm;
  RenderSystem renderSystem;
  MovementSystem movementSystem;
  HerdSystem herdSystem(50);

  Entity dog = em.createEntity();
  cm.addComponent(dog, PositionComponent{10, 10});
  cm.addComponent(dog, VelocityComponent{10, 10});
  cm.addComponent(dog, RenderComponent{10, 10, 255, 255, 255, 255});
  cm.addComponent(dog, HerdComponent{50, 1, 1, 1, 100});
  Entity cat = em.createEntity();
  cm.addComponent(cat, PositionComponent{20, 20});
  cm.addComponent(cat, VelocityComponent{10, 10});
  cm.addComponent(cat, RenderComponent{10, 10, 0, 0, 0, 255});
  cm.addComponent(cat, HerdComponent{50, 1, 1, 1, 100});

  Entity cat2 = em.createEntity();
  cm.addComponent(cat2, PositionComponent{20, 20});
  cm.addComponent(cat2, VelocityComponent{10, 10});
  cm.addComponent(cat2, RenderComponent{25, 10, 0, 255, 0, 255});
  cm.addComponent(cat2, HerdComponent{50, 1, 1, 1, 100});

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
    herdSystem.update(dt, cm.getComponentMap<PositionComponent>(),
                      cm.getComponentMap<VelocityComponent>(),
                      cm.getComponentMap<HerdComponent>());
    movementSystem.update(dt, cm.getComponentMap<PositionComponent>(),
                          cm.getComponentMap<VelocityComponent>());
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    tg.renderTileGrid(renderer, camera.getX(), camera.getY());
    tg.renderLineGrid(renderer, camera.getX(), camera.getY());

    int worldMouseX = input.getMouseX() + (int)camera.getX();
    int worldMouseY = input.getMouseY() + (int)camera.getY();
    tg.tileHighlight(renderer, worldMouseX, worldMouseY, camera.getX(),
                     camera.getY());

    tg.renderBrushIndicator(renderer);
    renderSystem.render(renderer, camera,
                        cm.getComponentMap<PositionComponent>(),
                        cm.getComponentMap<RenderComponent>());
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
