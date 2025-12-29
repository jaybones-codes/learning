#include "BoidSystem.h"
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
  ComponentManager cm;
  RenderSystem renderSystem;
  MovementSystem movementSystem;
  BoidSystem boidSystem; // Adjust the cell size as neededm;
  for (int i = 0; i < 100; i++) {
    Entity boid = em.createEntity();
    float inc = 20;
    inc *= i;

    // Source - https://stackoverflow.com/a
    // Posted by John Dibling, modified by community. See post 'Timeline' for
    // change history Retrieved 2025-12-28, License - CC BY-SA 3.0

    float r2 =
        static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100));
    float r1 =
        static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100));
    float randomVx = (rand() % 200) - 100; // -100 to 100
    float randomVy = (rand() % 200) - 100;
    std::cout << "Entity Created" << std::endl;
    cm.addComponent(boid, PositionComponent{r2, r1}); // Add Position component)
    cm.addComponent(boid, VelocityComponent{randomVx, randomVy});
    cm.addComponent(boid, RenderComponent{5, 6, 255, 0, 0, 255});
    cm.addComponent(boid, BoidComponent{25.0f,    // sep Radius
                                        30.0f,    // align radius
                                        100.0f,   // cohesion radius
                                        8.0f,     // sep weight
                                        3.0f,     // align weight
                                        0.2f,     // cohesion weight
                                        150.0f,   // maxspeed
                                        100.0f}); // maxforce
  }
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
    boidSystem.update(dt, cm.getComponentMap<BoidComponent>(),
                      cm.getComponentMap<PositionComponent>(),
                      cm.getComponentMap<VelocityComponent>());
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
