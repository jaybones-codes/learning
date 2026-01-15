#include "ECS.h"
#include <iostream>
#include <unordered_set>

// Entity Manager for creating entities.
//
Entity EntityManager::m_nextId = 0;
// EntityManager::EntityManager() {}
Entity EntityManager::createEntity() {

  if (!m_entityPool.empty()) {
    auto &last = m_entityPool.back();
    m_entityPool.pop_back();
    m_aliveEntities.insert(last);
    return last;
  }

  uint32_t id = m_nextId;
  m_nextId++;
  m_aliveEntities.insert(id);
  return id;
}
void EntityManager::destroyEntity(Entity entity) {
  m_entityPool.push_back(entity);
  m_deadEntities.push_back(entity);
  m_aliveEntities.erase(entity);
  std::cout << entity << "  was killed" << std::endl;
}

std::unordered_set<Entity> &EntityManager::getActiveEntities() {
  return m_aliveEntities;
}
bool EntityManager::isAlive(Entity entity) {
  return m_aliveEntities.count(entity) > 0;
}

void MovementSystem::update(
    float deltaTime, std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, VelocityComponent> &velocities) {

  for (auto &[entity, velocity] : velocities) {
    if (positions.count(entity)) {

      auto &position = positions[entity];
      position.x += velocity.vx * deltaTime;
      position.y += velocity.vy * deltaTime;
    }
  }
}
void RenderSystem::render(
    SDL_Renderer *renderer, Camera &camera,
    std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, RenderComponent> &renders) {

  for (auto &[entity, render] : renders) {
    if (positions.count(entity)) {
      auto &position = positions[entity];
      float x = position.x - camera.getX();
      float y = position.y - camera.getY();
      SDL_FRect rect = {x, y, render.w, render.h};
      SDL_SetRenderDrawColor(renderer, render.r, render.g, render.b, render.a);
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}
