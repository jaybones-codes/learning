#pragma once
#include "Camera.h"
#include <SDL3/SDL.h>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Entity = uint32_t;
struct Vec2 {
  float x;
  float y;
};
struct PositionComponent {
  float x;
  float y;
};

struct VelocityComponent {
  float vx;
  float vy;
};

struct RenderComponent {

  float w;
  float h;
  // Colours
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

struct PlayerInputComponent {

  float speed;
};

struct CollisionComponent {
  float w;
  float h;
};

struct CameraTargetComponent {};

struct BoidComponent {
  float weightScale = 1.0f;
  float radiusScale = 1.0f;
};
class EntityManager {
private:
  static Entity m_nextId;
  std::unordered_set<Entity> m_aliveEntities;
  std::vector<Entity> m_entityPool;
  std::vector<Entity> m_deadEntities;

public:
  Entity createEntity();
  void destroyEntity(Entity entity);
  bool isAlive(Entity entity);
  std::unordered_set<Entity> &getActiveEntities();
  void clearDeadEntities() { m_deadEntities.clear(); }
  const std::vector<Entity> &getDeadEntities() { return m_deadEntities; }
  std::vector<Entity> &getPool() { return m_entityPool; }
};

class ComponentManager {
private:
  std::unordered_map<Entity, PositionComponent> m_positions;
  std::unordered_map<Entity, VelocityComponent> m_velocities;
  std::unordered_map<Entity, RenderComponent> m_renders;
  std::unordered_map<Entity, PlayerInputComponent> m_playerInput;
  std::unordered_map<Entity, BoidComponent> m_boids;
  std::unordered_map<Entity, CameraTargetComponent> m_cameraTargets;
  std::unordered_map<Entity, CollisionComponent> m_collisions;

public:
  void removeAllComponents(Entity e) {
    removeComponent<PositionComponent>(e);
    removeComponent<VelocityComponent>(e);
    removeComponent<RenderComponent>(e);
    removeComponent<PlayerInputComponent>(e);
    removeComponent<BoidComponent>(e);

    removeComponent<CameraTargetComponent>(e);
  }

  template <typename T> std::unordered_map<Entity, T> &getComponentMap();

  template <typename T> void addComponent(Entity entity, T component) {
    getComponentMap<T>()[entity] = component;
  }

  template <typename T> bool hasComponent(Entity entity) {
    return getComponentMap<T>().count(entity) > 0;
  }

  template <typename T> T &getComponent(Entity entity) {
    return getComponentMap<T>()[entity];
  }

  template <typename T> void removeComponent(Entity entity) {
    getComponentMap<T>().erase(entity);
  }
};

template <>
inline std::unordered_map<Entity, PositionComponent> &
ComponentManager::getComponentMap<PositionComponent>() {
  return m_positions;
}

template <>
inline std::unordered_map<Entity, VelocityComponent> &
ComponentManager::getComponentMap<VelocityComponent>() {
  return m_velocities;
}
template <>
inline std::unordered_map<Entity, RenderComponent> &
ComponentManager::getComponentMap<RenderComponent>() {
  return m_renders;
}
template <>
inline std::unordered_map<Entity, PlayerInputComponent> &
ComponentManager::getComponentMap<PlayerInputComponent>() {
  return m_playerInput;
}
template <>
inline std::unordered_map<Entity, BoidComponent> &
ComponentManager::getComponentMap<BoidComponent>() {
  return m_boids;
}
template <>
inline std::unordered_map<Entity, CameraTargetComponent> &
ComponentManager::getComponentMap<CameraTargetComponent>() {
  return m_cameraTargets;
}
template <>
inline std::unordered_map<Entity, CollisionComponent> &
ComponentManager::getComponentMap<CollisionComponent>() {
  return m_collisions;
}
class MovementSystem {

public:
  void update(float deltaTime,
              std::unordered_map<Entity, PositionComponent> &positions,
              std::unordered_map<Entity, VelocityComponent> &velocities);
};
class RenderSystem {
public:
  void render(SDL_Renderer *renderer, Camera &camera,
              std::unordered_map<Entity, PositionComponent> &positions,
              std::unordered_map<Entity, RenderComponent> &renders);
};
