#include "PlayerInputSystem.h"

void PlayerInputSystem::update(
    std::unordered_map<Entity, PlayerInputComponent> &playerInputs,
    std::unordered_map<Entity, VelocityComponent> &velocities) {

  const auto *keys = SDL_GetKeyboardState(nullptr);

  for (auto &[entity, input] : playerInputs) {
    auto &velocity = velocities[entity];

    velocity.vx = 0;
    velocity.vy = 0;

    if (keys[SDL_SCANCODE_UP])
      velocity.vy -= input.speed;
    if (keys[SDL_SCANCODE_DOWN])
      velocity.vy += input.speed;
    if (keys[SDL_SCANCODE_LEFT])
      velocity.vx -= input.speed;
    if (keys[SDL_SCANCODE_RIGHT])
      velocity.vx += input.speed;
  }
}
