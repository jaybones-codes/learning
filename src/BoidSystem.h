#pragma once
#include <cstdint>
#include <unordered_map>
struct PositionComponent;
struct VelocityComponent;
struct BoidComponent;
struct Vec2;
using Entity = uint32_t;
#include "SpatialGrid.h"
class BoidSystem {
private:
  SpatialGrid m_spatialGrid;

public:
  BoidSystem(float worldWidth, float worldHeight, float cellSize);

  void update(float deltaTime, std::unordered_map<Entity, BoidComponent> &boids,
              std::unordered_map<Entity, PositionComponent> &positions,
              std::unordered_map<Entity, VelocityComponent> &velocities);

private:
  Vec2
  calculateSeparation(Entity entity, VelocityComponent &entityVelocity,
                      PositionComponent &entityPosition,
                      BoidComponent &entityBoid,
                      std::unordered_map<Entity, PositionComponent> &positions,
                      std::unordered_map<Entity, BoidComponent> &boids);

  Vec2
  calculateAlignment(Entity entity, PositionComponent &entityPosition,
                     VelocityComponent &entityVelocity,
                     BoidComponent &entityBoid,
                     std::unordered_map<Entity, PositionComponent> &positions,
                     std::unordered_map<Entity, BoidComponent> &boids,
                     std::unordered_map<Entity, VelocityComponent> &velocities);
  Vec2
  calculateCohesion(Entity entity, PositionComponent &entityPosition,
                    VelocityComponent &entityVelocity,
                    BoidComponent &entityBoid,
                    std::unordered_map<Entity, PositionComponent> &positions,
                    std::unordered_map<Entity, BoidComponent> &boids);
};

float getDistance(PositionComponent a, PositionComponent b);
Vec2 limit(Vec2 vec, float max);
Vec2 normalize(Vec2 vec);
