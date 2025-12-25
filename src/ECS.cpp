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
  m_aliveEntities.erase(entity);
  std::cout << entity << "  was killed" << std::endl;
}

std::unordered_set<Entity> &EntityManager::getActiveEntities() {
  return m_aliveEntities;
}
bool EntityManager::isAlive(Entity entity) {
  return m_aliveEntities.count(entity) > 0;
}

// Component Manager for handling components.
