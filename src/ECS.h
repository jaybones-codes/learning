#pragma once
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using Entity = uint32_t;

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

class EntityManager {
private:
  static Entity m_nextId;
  std::unordered_set<Entity> m_aliveEntities;
  std::vector<Entity> m_entityPool;

public:
  Entity createEntity();
  void destroyEntity(Entity entity);
  bool isAlive(Entity entity);
  std::unordered_set<Entity> &getActiveEntities();
};

class ComponentManager {
private:
  std::unordered_map<Entity, PositionComponent> m_positions;
  std::unordered_map<Entity, VelocityComponent> m_velocities;
  std::unordered_map<Entity, RenderComponent> m_renders;

public:
  template <typename T> std::unordered_map<Entity, T> &getComponentMap();

  template <typename T> void addComponent(Entity entity, T component) {
    getComponentMap<T>()[entity] = component;
  }
};

// Still in the .h file, after the class:
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
