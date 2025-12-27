#include "HerdSystem.h"
#include "ECS.h"
#include <unordered_map>
#include <vector>

SpatialGrid::SpatialGrid(float size) {};
void SpatialGrid::clear() { cells.clear(); };
void SpatialGrid::insert(Entity e, float x, float y) {
  int cellX = (int)(x / cellSize);
  int cellY = (int)(y / cellSize);

  cells[cellX][cellY].push_back(e);
}

HerdSystem::HerdSystem(float cellSize)
    : grid(cellSize) {

      };
void HerdSystem::update(
    float deltaTime, std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, VelocityComponent> &velocities,
    std::unordered_map<Entity, HerdComponent> &herds) {

  grid.clear();
  float sumX = 0.0f;
  float sumY = 0.0f;
  int count = 0;

  for (auto &[entity, herd] : herds) {
    auto pit = positions.find(entity);
    if (pit != positions.end()) {
      grid.insert(entity, pit->second.x, pit->second.y);
    }
  }
  for (auto &[entity, herd] : herds) {

    if (!positions.count(entity))
      continue;
    if (!velocities.count(entity))
      continue;

    auto &pos = positions[entity];
    auto &vel = velocities[entity];

    // neighbor search goes here
    int cellX = (int)(pos.x / grid.cellSize);
    int cellY = (int)(pos.y / grid.cellSize);

    for (int dy = -1; dy <= 1; dy++) {
      for (int dx = -1; dx <= 1; dx++) {

        int nx = cellX + dx;
        int ny = cellY + dy;

        if (!grid.cells.count(nx))
          continue;
        if (!grid.cells[nx].count(ny))
          continue;

        for (Entity other : grid.cells[nx][ny]) {

          if (other == entity)
            continue;
          if (!positions.count(other))
            continue;

          auto &otherPos = positions[other];

          float dx = otherPos.x - pos.x;
          float dy = otherPos.y - pos.y;
          float dist2 = dx * dx + dy * dy;

          if (dist2 > herd.perceptionRadius * herd.perceptionRadius)
            continue;

          // neighbor accepted
          sumX += otherPos.x;
          sumY += otherPos.y;
          count++;
        }
      }
      if (count > 0) {
        float avgX = sumX / count;
        float avgY = sumY / count;

        float steerX = avgX - pos.x;
        float steerY = avgY - pos.y;

        vel.vx += steerX * deltaTime;
        vel.vy += steerY * deltaTime;
      }
    }
  }
};
