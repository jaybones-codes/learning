#include "BoidSystem.h"
#include "Camera.h"
#include "CameraTargetSystem.h"
#include "ECS.h"
#include "Init.h"
#include "Input.h"
#include "PlayerInputSystem.h"
#include "Systems.h"
#include "TileGrid.h"
#include "TimeManager.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "imgui.h"
#include <SDL3/SDL.h>
#include <Simulation.h>
#include <iostream>
int main() {
  EngineInit init;
  Simulation sim;

  EntityManager em;
  bool isRunning = true;
  TimeManager time;
  Input input;
  TileGrid tg;
  Camera camera(sim.SCREENWIDTH, sim.SCREENHEIGHT, tg.GRID_WIDTH * tg.TILE_SIZE,
                tg.GRID_HEIGHT * tg.TILE_SIZE);
  PlayerInputSystem playerInputSystem;
  ComponentManager cm;
  RenderSystem renderSystem;
  MovementSystem movementSystem;
  CameraTargetSystem targetSystem;
  CollisionSystem collisionSystem;
  Entity player = em.createEntity();
  cm.addComponent(player, PositionComponent{50, 50});
  cm.addComponent(player, VelocityComponent{0, 0});
  cm.addComponent(player, PlayerInputComponent{200.0f});
  cm.addComponent(player, RenderComponent{32, 32, 255, 200, 0, 255});
  cm.addComponent(player, CameraTargetComponent{});
  cm.addComponent(player, CollisionComponent{64, 64});
  BoidSystem boidSystem(800, 600, 100); // Adjust the cell size as neededm;
  for (int i = 0; i < 20; i++) {
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
    cm.addComponent(boid, BoidComponent{1, 1}); // maxforce
    cm.addComponent(boid, CollisionComponent{5, 6});
  }
  int i = (int)em.getActiveEntities().size();

  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event); // ADD THIS FIRST

      // Then your event handling
      input.processEvent(event);
      if (event.type == SDL_EVENT_QUIT ||
          input.isKeyDown(SDL_SCANCODE_ESCAPE)) {
        isRunning = false;
      }
    }
    time.updateDeltaTime();
    // Start ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    float dt = time.getDeltaTime();
    targetSystem.update(camera, cm.getComponentMap<PositionComponent>(),
                        cm.getComponentMap<CameraTargetComponent>());
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
    playerInputSystem.update(cm.getComponentMap<PlayerInputComponent>(),
                             cm.getComponentMap<VelocityComponent>());
    boidSystem.update(dt, cm.getComponentMap<BoidComponent>(),
                      cm.getComponentMap<PositionComponent>(),
                      cm.getComponentMap<VelocityComponent>());
    movementSystem.update(dt, cm.getComponentMap<PositionComponent>(),
                          cm.getComponentMap<VelocityComponent>());

    collisionSystem.update(tg, cm.getComponentMap<PositionComponent>(),
                           cm.getComponentMap<CollisionComponent>(),
                           cm.getComponentMap<VelocityComponent>());
    SDL_SetRenderDrawColor(init.getRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(init.getRenderer());
    ImGui::Begin("Debug Info");

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Delta Time: %.3f ms", dt * 1000.0f);
    ImGui::Text("Entity Count: %d", (int)em.getActiveEntities().size());
    ImGui::Text("Camera: (%.1f, %.1f)", camera.getX(), camera.getY());
    ImGui::Text("Dead Entities: %d", (int)em.getPool().size());
    ImGui::Separator(); // Visual divider

    ImGui::Text("Mouse: (%d, %d)", (int)input.getMouseX(),
                (int)input.getMouseY());
    ImGui::End();
    if (ImGui::Begin("Boid Parameters")) {

      BoidSpecs &specs = boidSystem.getSpecs();

      ImGui::SliderFloat("Cohesion Weight", &specs.cohesionWeight, 0.0f, 5.0f);
      ImGui::SliderFloat("Separation Weight", &specs.separationWeight, 0.0f,
                         10.0f);
      ImGui::SliderFloat("Alignment Weight", &specs.alignmentWeight, 0.0f,
                         5.0f);

      ImGui::SliderFloat("Separation Radius", &specs.separationRadius, 0.0f,
                         200.0f);
      ImGui::SliderFloat("Alignment Radius", &specs.alignmentRadius, 0.0f,
                         200.0f);
      ImGui::SliderFloat("Cohesion Radius", &specs.cohesionRadius, 0.0f,
                         200.0f);

      ImGui::SliderFloat("Max Speed", &specs.maxSpeed, 0.0f, 500.0f);
      ImGui::SliderFloat("Max Force", &specs.maxForce, 0.0f, 500.0f);

      ImGui::SliderFloat("Boundary Radius", &specs.boundaryRadius, 0.0f,
                         500.0f);
      ImGui::SliderFloat("Boundary Weight", &specs.boundaryWeight, 0.0f, 20.0f);

      ImGui::End();
    }
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
    sim.cleanupDeadEntities(em, cm);
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
