#include "BoidSystem.h"
#include "Camera.h"
#include "ECS.h"
#include "Init.h"
#include "Input.h"
#include "TileGrid.h"
#include "TimeManager.h"
#include <SDL3/SDL.h>
#include <iostream>

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "imgui.h"
int main() {
  EngineInit init;

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
  BoidSystem boidSystem(800, 600, 100); // Adjust the cell size as neededm;
  for (int i = 0; i < 200; i++) {
    Entity boid = em.createEntity();
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
      ImGui_ImplSDL3_ProcessEvent(&event); // ADD THIS FIRST

      // Then your event handling
      input.processEvent(event);
      if (event.type == SDL_EVENT_QUIT) {
        isRunning = false;
      }
    }
    time.updateDeltaTime();
    // Start ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
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
    SDL_SetRenderDrawColor(init.getRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(init.getRenderer());
    ImGui::Begin("Debug Info");

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Delta Time: %.3f ms", dt * 1000.0f);
    ImGui::Text("Entity Count: %d", (int)em.getActiveEntities().size());
    ImGui::Text("Camera: (%.1f, %.1f)", camera.getX(), camera.getY());

    ImGui::Separator(); // Visual divider

    ImGui::Text("Mouse: (%d, %d)", (int)input.getMouseX(),
                (int)input.getMouseY());
    ImGui::End();
    ImGui::Render();
    tg.renderTileGrid(init.getRenderer(), camera.getX(), camera.getY());
    tg.renderLineGrid(init.getRenderer(), camera.getX(), camera.getY());
    int worldMouseX = input.getMouseX() + (int)camera.getX();
    int worldMouseY = input.getMouseY() + (int)camera.getY();
    tg.tileHighlight(init.getRenderer(), worldMouseX, worldMouseY,
                     camera.getX(), camera.getY());

    tg.renderBrushIndicator(init.getRenderer());
    renderSystem.render(init.getRenderer(), camera,

                        cm.getComponentMap<PositionComponent>(),
                        cm.getComponentMap<RenderComponent>());
    // Render ImGui on top
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(),
                                          init.getRenderer());
    SDL_RenderPresent(init.getRenderer());
  }
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
  std::cout << "ImGui Shutdown" << std::endl;
  SDL_DestroyRenderer(init.getRenderer());
  SDL_DestroyWindow(init.getWindow());
  SDL_Quit();
  std::cout << "SDL Shutdown" << std::endl;

  return 0;
}
