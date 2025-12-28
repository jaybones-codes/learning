
#include "HerdSystem.h"
#include "ECS.h"
#include <cmath>
#include <unordered_map>
#include <vector>

// ------------------- SpatialGrid -------------------

SpatialGrid::SpatialGrid(float size) : cellSize(size) {}

void SpatialGrid::clear() { cells.clear(); }

void SpatialGrid::insert(Entity e, float x, float y) {
  int cellX = (int)(x / cellSize);
  int cellY = (int)(y / cellSize);
  cells[cellX][cellY].push_back(e);
}

// ------------------- HerdSystem -------------------

HerdSystem::HerdSystem(float cellSize) : grid(cellSize) {}

void HerdSystem::update(
    float deltaTime, std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, VelocityComponent> &velocities,
    std::unordered_map<Entity, HerdComponent> &herds) {
  grid.clear();

  // Insert all entities into the spatial grid
  for (auto &[entity, herd] : herds) {
    auto pit = positions.find(entity);
    if (pit != positions.end()) {
      grid.insert(entity, pit->second.x, pit->second.y);
    }
  }

  // Update each entity
  for (auto &[entity, herd] : herds) {
    auto posIt = positions.find(entity);
    auto velIt = velocities.find(entity);
    if (posIt == positions.end() || velIt == velocities.end())
      continue;

    auto &pos = posIt->second;
    auto &vel = velIt->second;

    // Neighbor accumulators
    float cohesionSumX = 0.0f, cohesionSumY = 0.0f;
    float alignmentSumX = 0.0f, alignmentSumY = 0.0f;
    float separationX = 0.0f, separationY = 0.0f;
    int neighborCount = 0;

    int cellX = (int)(pos.x / grid.cellSize);
    int cellY = (int)(pos.y / grid.cellSize);

    for (int dy = -1; dy <= 1; dy++) {
      for (int dx = -1; dx <= 1; dx++) {
        int nx = cellX + dx;
        int ny = cellY + dy;

        auto colIt = grid.cells.find(nx);
        if (colIt == grid.cells.end())
          continue;
        auto rowIt = colIt->second.find(ny);
        if (rowIt == colIt->second.end())
          continue;

        for (Entity other : rowIt->second) {
          if (other == entity)
            continue;
          auto otherPosIt = positions.find(other);
          auto otherVelIt = velocities.find(other);
          if (otherPosIt == positions.end() || otherVelIt == velocities.end())
            continue;

          auto &otherPos = otherPosIt->second;
          auto &otherVel = otherVelIt->second;

          float dx = otherPos.x - pos.x;
          float dy = otherPos.y - pos.y;
          float dist2 = dx * dx + dy * dy;

          if (dist2 > herd.perceptionRadius * herd.perceptionRadius)
            continue;

          neighborCount++;

          // Cohesion
          cohesionSumX += otherPos.x;
          cohesionSumY += otherPos.y;

          // Alignment
          alignmentSumX += otherVel.vx;
          alignmentSumY += otherVel.vy;

          // Separation (repulsion)
          if (dist2 > 0.0f) {
            separationX -= dx / dist2; // inversely proportional to distance
            separationY -= dy / dist2;
          }
        }
      }
    }

    if (neighborCount > 0) {
      // Cohesion
      float avgX = cohesionSumX / neighborCount;
      float avgY = cohesionSumY / neighborCount;
      vel.vx += (avgX - pos.x) * herd.cohesionWeight * deltaTime;
      vel.vy += (avgY - pos.y) * herd.cohesionWeight * deltaTime;

      // Alignment
      float avgVelX = alignmentSumX / neighborCount;
      float avgVelY = alignmentSumY / neighborCount;
      vel.vx += (avgVelX - vel.vx) * herd.alignmentWeight * deltaTime;
      vel.vy += (avgVelY - vel.vy) * herd.alignmentWeight * deltaTime;

      // Separation
      vel.vx += separationX * herd.separationWeight * deltaTime;
      vel.vy += separationY * herd.separationWeight * deltaTime;

      // Clamp speed
      float speed = std::sqrt(vel.vx * vel.vx + vel.vy * vel.vy);
      if (speed > herd.maxSpeed) {
        vel.vx = (vel.vx / speed) * herd.maxSpeed;
        vel.vy = (vel.vy / speed) * herd.maxSpeed;
      }
    }
  }
}
